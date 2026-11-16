/*
 * tempo.c — medição de tempo e saída CSV.
 * Responsável: Pedro Salles · Revisor: Augusto
 */
#define _POSIX_C_SOURCE 200809L   /* libera o clock_gettime */
#include <stdio.h>
#include <time.h>
#include "tempo.h"

double agora(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec * 1e-9;
}

void imprime_csv(const char *versao, int P, unsigned long long bytes_in,
                 unsigned long long bytes_out, double tempo)
{
    printf("%s,%d,%llu,%llu,%.6f\n", versao, P, bytes_in, bytes_out, tempo);
}
