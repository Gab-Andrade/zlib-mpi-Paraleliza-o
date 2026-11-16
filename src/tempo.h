/*
 * tempo.h — medição de tempo e saída CSV (usado pela serial e pela MPI).
 * Responsável: Pedro Salles · Revisor: Augusto
 */
#ifndef TEMPO_H
#define TEMPO_H

/* Relógio em segundos (monotônico: não pula se o relógio do PC mudar). */
double agora(void);

/* Imprime: versao,P,bytes_entrada,bytes_saida,tempo */
void imprime_csv(const char *versao, int P, unsigned long long bytes_in,
                 unsigned long long bytes_out, double tempo);

#endif
