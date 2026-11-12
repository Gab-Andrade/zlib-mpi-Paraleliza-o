/*
 * zpipe_serial.c — versão serial (linha de base T1).
 * Adaptado do exemplo oficial da zlib, examples/zpipe.c (Mark Adler).
 * Mudanças: gera .gz (15 + 16), recebe arquivos por argumento e mede o tempo.
 *
 * Uso: zpipe_serial entrada saida
 */
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <zlib.h>

#define CHUNK (256 * 1024)

static double agora(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec * 1e-9;
}

int main(int argc, char **argv)
{
    static unsigned char in[CHUNK], out[CHUNK];

    if (argc != 3) {
        fprintf(stderr, "uso: %s entrada saida\n", argv[0]);
        return 1;
    }
    FILE *src = fopen(argv[1], "rb");
    FILE *dst = fopen(argv[2], "wb");
    if (!src || !dst) {
        fprintf(stderr, "erro ao abrir os arquivos\n");
        return 1;
    }

    z_stream strm;
    memset(&strm, 0, sizeof strm);
    /* 15 = janela de 32 KB; +16 = gerar formato gzip */
    if (deflateInit2(&strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                     15 + 16, 8, Z_DEFAULT_STRATEGY) != Z_OK) {
        fprintf(stderr, "erro no deflateInit2\n");
        return 1;
    }

    double t0 = agora();
    int flush;
    do {                                   /* lê um pedaço da entrada */
        strm.avail_in = fread(in, 1, CHUNK, src);
        if (ferror(src)) { fprintf(stderr, "erro de leitura\n"); return 1; }
        flush = feof(src) ? Z_FINISH : Z_NO_FLUSH;
        strm.next_in = in;

        do {                               /* comprime até esvaziar a entrada */
            strm.avail_out = CHUNK;
            strm.next_out = out;
            deflate(&strm, flush);
            size_t have = CHUNK - strm.avail_out;
            if (fwrite(out, 1, have, dst) != have) {
                fprintf(stderr, "erro de escrita\n");
                return 1;
            }
        } while (strm.avail_out == 0);
    } while (flush != Z_FINISH);

    if (fclose(dst) != 0) { fprintf(stderr, "erro ao fechar a saída\n"); return 1; }
    fclose(src);
    double t = agora() - t0;
    deflateEnd(&strm);

    printf("serial,%.6f\n", t);           /* tempo em segundos */
    return 0;
}