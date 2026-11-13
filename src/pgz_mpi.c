/*
 * pgz_mpi.c — compressão gzip em blocos com MPI.
 * Responsável: Gabriel (comunicação MPI) · Revisor: Renan
 *
 * Uso: mpirun -np P ./pgz_mpi entrada saida
 */
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "bloco.h"
#include "tempo.h"

#define B (1024 * 1024)   /* tamanho de um bloco: 1 MB */

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    int rank, P;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &P);

    if (argc != 3) {
        if (rank == 0)
            fprintf(stderr, "uso: mpirun -np P %s entrada saida\n", argv[0]);
        MPI_Finalize();
        return 1;
    }

    /* Todos os ranks: compressor, buffer do bloco e buffer comprimido. */
    z_stream strm;
    if (bloco_inicia(&strm) != 0) {
        fprintf(stderr, "rank %d: erro ao iniciar a zlib\n", rank);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    int cap = bloco_cap(&strm, B);
    unsigned char *meu_bloco = malloc(B);
    unsigned char *comprimido = malloc(cap);
    if (!meu_bloco || !comprimido) {
        fprintf(stderr, "rank %d: sem memória\n", rank);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    /* Só o rank 0: arquivos, buffers da rodada e listas do Scatterv/Gatherv. */
    FILE *src = NULL, *dst = NULL;
    unsigned char *rodada = NULL, *saida = NULL;
    int *conts = NULL, *desl = NULL, *rconts = NULL, *rdesl = NULL;
    unsigned long *infos = NULL;
    if (rank == 0) {
        src = fopen(argv[1], "rb");
        dst = fopen(argv[2], "wb");
        rodada = malloc((size_t)P * B);
        saida  = malloc((size_t)P * cap);
        conts  = malloc(P * sizeof(int));
        desl   = malloc(P * sizeof(int));
        rconts = malloc(P * sizeof(int));
        rdesl  = malloc(P * sizeof(int));
        infos  = malloc(2 * P * sizeof(unsigned long));
        if (!src || !dst || !rodada || !saida || !conts || !desl ||
            !rconts || !rdesl || !infos) {
            fprintf(stderr, "erro ao abrir os arquivos ou alocar memória\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        if (gzip_cabecalho(dst) != 0) {               /* começo do .gz */
            fprintf(stderr, "erro ao gravar o cabeçalho\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
    }

    unsigned long long bytes_out = 20;    /* cabeçalho (10) + fim do gzip (10) */
    MPI_Barrier(MPI_COMM_WORLD);          /* todos começam o cronômetro juntos */
    double t0 = agora();
    unsigned long crc_total = crc32(0L, Z_NULL, 0);   /* só o rank 0 usa */
    unsigned long long total = 0;                     /* bytes originais */

    while (1) {
        /* 1. Rank 0 lê até P blocos. */
        int n = 0;
        if (rank == 0)
            n = (int)fread(rodada, 1, (size_t)P * B, src);

        /* 2. Rank 0 avisa todos quantos bytes leu. */
        MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
        if (n == 0)
            break;

        int meu = n - rank * B;
        if (meu < 0) meu = 0;
        if (meu > B) meu = B;

        if (rank == 0) {
            for (int i = 0; i < P; i++) {
                int c = n - i * B;
                if (c < 0) c = 0;
                if (c > B) c = B;
                conts[i] = c;
                desl[i] = i * B;
            }
        }

        /* 3. Distribui: o bloco i vai para o rank i. */
        MPI_Scatterv(rodada, conts, desl, MPI_UNSIGNED_CHAR,
                     meu_bloco, meu, MPI_UNSIGNED_CHAR,
                     0, MPI_COMM_WORLD);

        /* 4. Cada rank comprime o seu bloco. */
        int comp_len;
        unsigned long crc;
        if (bloco_comprime(&strm, meu_bloco, meu, comprimido, cap,
                           &comp_len, &crc) != 0) {
            fprintf(stderr, "rank %d: erro ao comprimir\n", rank);
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        /* 5. Todos mandam ao rank 0: {tamanho comprimido, crc}. */
        unsigned long info[2] = {(unsigned long)comp_len, crc};
        MPI_Gather(info, 2, MPI_UNSIGNED_LONG,
                   infos, 2, MPI_UNSIGNED_LONG, 0, MPI_COMM_WORLD);

        /* Rank 0 já sabe os tamanhos: monta onde cada pedaço vai cair. */
        int total_comp = 0;
        if (rank == 0) {
            for (int i = 0; i < P; i++) {
                rconts[i] = (int)infos[2 * i];
                rdesl[i] = total_comp;               /* um logo após o outro */
                total_comp += rconts[i];
            }
        }

        /* 6. Todos mandam os bytes comprimidos; chegam em ordem. */
        MPI_Gatherv(comprimido, comp_len, MPI_UNSIGNED_CHAR,
                    saida, rconts, rdesl, MPI_UNSIGNED_CHAR,
                    0, MPI_COMM_WORLD);

        /* 7. Rank 0 grava e junta os CRCs EM ORDEM (bloco 0, 1, 2...). */
        if (rank == 0) {
            if (fwrite(saida, 1, total_comp, dst) != (size_t)total_comp) {
                fprintf(stderr, "erro ao gravar a saída\n");
                MPI_Abort(MPI_COMM_WORLD, 1);
            }
            for (int i = 0; i < P; i++)
                if (conts[i] > 0)
                    crc_total = crc32_combine(crc_total, infos[2 * i + 1], conts[i]);
            total += n;
            bytes_out += total_comp;
        }
    }

    /* Fim do .gz: bloco final vazio + CRC total + tamanho original. */
    if (rank == 0) {
        if (gzip_final(dst, crc_total, total) != 0 || fclose(dst) != 0) {
            fprintf(stderr, "erro ao finalizar a saída\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        fclose(src);
        double t = agora() - t0;
        imprime_csv("mpi", P, total, bytes_out, t);
    }

    bloco_fim(&strm);
    free(meu_bloco);
    free(comprimido);
    if (rank == 0) {
        free(rodada); free(saida);
        free(conts); free(desl); free(rconts); free(rdesl); free(infos);
    }
    MPI_Finalize();
    return 0;
}