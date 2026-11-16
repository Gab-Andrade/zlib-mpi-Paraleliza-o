/*
 * zpipe_serial.c — versão serial (linha de base T1).
 * Responsável: Pedro Salles · Revisor: Augusto
 *
 * Adaptado do exemplo oficial da zlib, examples/zpipe.c (Mark Adler).
 * Uso: zpipe_serial entrada saida
 */
#include <stdio.h>
#include <string.h>
#include <zlib.h>
#include "tempo.h"

#define CHUNK (256 * 1024)

/* Comprime src inteiro em dst (formato gzip). Devolve 0 se deu certo. */
static int comprime_arquivo(z_stream *strm, FILE *src, FILE *dst,
                            unsigned long long *bytes_in,
                            unsigned long long *bytes_out)
{
    static unsigned char in[CHUNK], out[CHUNK];
    int ret = Z_OK, flush;

    do {                                   /* lê um pedaço da entrada */
        strm->avail_in = fread(in, 1, CHUNK, src);
        if (ferror(src))
            return -1;
        *bytes_in += strm->avail_in;
        flush = feof(src) ? Z_FINISH : Z_NO_FLUSH;
        strm->next_in = in;

        do {                               /* comprime até esvaziar a entrada */
            strm->avail_out = CHUNK;
            strm->next_out = out;
            ret = deflate(strm, flush);
            if (ret == Z_STREAM_ERROR)
                return -1;
            size_t have = CHUNK - strm->avail_out;
            if (fwrite(out, 1, have, dst) != have)
                return -1;
            *bytes_out += have;
        } while (strm->avail_out == 0);
    } while (flush != Z_FINISH);

    return ret == Z_STREAM_END ? 0 : -1;   /* terminou direito? */
}

int main(int argc, char **argv)
{
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

    unsigned long long bytes_in = 0, bytes_out = 0;
    double t0 = agora();                   /* ler + comprimir + gravar + fechar */
    int erro = comprime_arquivo(&strm, src, dst, &bytes_in, &bytes_out);
    if (fclose(dst) != 0)
        erro = -1;
    fclose(src);
    double t = agora() - t0;
    deflateEnd(&strm);

    if (erro) {
        fprintf(stderr, "erro ao comprimir\n");
        return 1;
    }
    imprime_csv("serial", 1, bytes_in, bytes_out, t);
    return 0;
}
