/*
 * bloco.c — compressão de um bloco e formato gzip.
 * Responsável: Matheus · Revisor: Augusto
 */
#include <string.h>
#include "bloco.h"

int bloco_inicia(z_stream *strm)
{
    memset(strm, 0, sizeof *strm);
    /* -15: deflate "cru", sem cabeçalho nem trailer.
     * O cabeçalho e o fim do gzip são gravados à mão pelo rank 0. */
    return deflateInit2(strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                        -15, 8, Z_DEFAULT_STRATEGY) == Z_OK ? 0 : -1;
}

void bloco_fim(z_stream *strm)
{
    deflateEnd(strm);
}

int bloco_cap(z_stream *strm, int n)
{
    /* deflateBound = pior caso da zlib; +16 de folga para o SYNC_FLUSH. */
    return (int)deflateBound(strm, (uLong)n) + 16;
}

int bloco_comprime(z_stream *strm, const unsigned char *in, int n,
                   unsigned char *out, int cap,
                   int *out_len, unsigned long *crc)
{
    *out_len = 0;
    *crc = crc32(0L, Z_NULL, 0);          /* CRC de "nada" */
    if (n == 0)
        return 0;                         /* rank sem bloco nesta rodada */

    deflateReset(strm);                   /* começa sem "lembrar" do bloco anterior */
    strm->next_in = (Bytef *)in;
    strm->avail_in = (uInt)n;
    strm->next_out = out;
    strm->avail_out = (uInt)cap;

    /* SYNC_FLUSH: termina alinhado em byte e NÃO marca como último bloco. */
    int ret = deflate(strm, Z_SYNC_FLUSH);
    if (ret != Z_OK || strm->avail_in != 0 || strm->avail_out == 0)
        return -1;                        /* erro ou faltou espaço */

    *out_len = cap - (int)strm->avail_out;
    *crc = crc32(*crc, in, (uInt)n);
    return 0;
}

/* Grava v em 4 bytes, do menos significativo para o mais (little-endian). */
static void le32(unsigned char *p, unsigned long v)
{
    p[0] = v & 0xff;
    p[1] = (v >> 8) & 0xff;
    p[2] = (v >> 16) & 0xff;
    p[3] = (v >> 24) & 0xff;
}

int gzip_cabecalho(FILE *f)
{
    /* 1f 8b = "sou gzip"; 08 = deflate; resto zerado; 03 = Unix */
    static const unsigned char cab[10] = {0x1f, 0x8b, 0x08, 0, 0, 0, 0, 0, 0, 0x03};
    return fwrite(cab, 1, 10, f) == 10 ? 0 : -1;
}

int gzip_final(FILE *f, unsigned long crc, unsigned long long total)
{
    unsigned char fim[10];
    fim[0] = 0x03;                        /* bloco final vazio: 03 00 */
    fim[1] = 0x00;
    le32(fim + 2, crc);                   /* CRC-32 do arquivo inteiro */
    le32(fim + 6, (unsigned long)(total & 0xffffffffULL)); /* tamanho mod 2^32 */
    return fwrite(fim, 1, 10, f) == 10 ? 0 : -1;
}