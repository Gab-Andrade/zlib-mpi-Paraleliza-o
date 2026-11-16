/*
 * bloco.h — compressão de um bloco e formato gzip.
 * Responsável: Matheus · Revisor: Augusto
 *
 * Contrato usado pelo pgz_mpi.c (Gabriel).
 */
#ifndef BLOCO_H
#define BLOCO_H

#include <stdio.h>
#include <zlib.h>

/* Prepara o z_stream (uma vez por rank). Devolve 0 se deu certo. */
int bloco_inicia(z_stream *strm);

/* Libera o z_stream no fim. */
void bloco_fim(z_stream *strm);

/* Tamanho máximo que um bloco de n bytes pode ocupar depois de comprimido.
 * Serve para alocar o buffer de saída. */
int bloco_cap(z_stream *strm, int n);

/* Comprime n bytes de 'in' para 'out' (capacidade 'cap').
 * Devolve em *out_len o tamanho comprimido e em *crc o CRC-32 do bloco.
 * Devolve 0 se deu certo. */
int bloco_comprime(z_stream *strm, const unsigned char *in, int n,
                   unsigned char *out, int cap,
                   int *out_len, unsigned long *crc);

/* Grava o cabeçalho gzip (10 bytes). Devolve 0 se deu certo. */
int gzip_cabecalho(FILE *f);

/* Grava o fim do gzip: bloco final vazio + CRC total + tamanho original.
 * Devolve 0 se deu certo. */
int gzip_final(FILE *f, unsigned long crc, unsigned long long total);

#endif