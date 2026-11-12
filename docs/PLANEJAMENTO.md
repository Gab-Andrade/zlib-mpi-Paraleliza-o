# Planejamento — zlib MPI (Seminário GSI059)

**Disciplina:** Programação Paralela — prof. Gilberto Nakamura
**Tema:** paralelizar com MPI a compressão gzip da zlib (`examples/zpipe.c`)
**Gestor:** Gabriel
**Última atualização:** 12/11/2026

> Documento vivo: atualizado pelo gestor ao longo da semana.

## Objetivo
Comprimir um arquivo em blocos, em paralelo com MPI, gerando um `.gz` válido
(que o `gunzip` comum abra), e medir o ganho de desempenho em relação à
versão serial: S(P) = T1 / TP.

## Papéis

| Segmento                    | Integrante(s)                  | Entregável                      |
|-----------------------------|--------------------------------|---------------------------------|
| Gestão e planejamento       | Gabriel                        | este plano de uma página        |
| Arquitetura paralela        | Gabriel                        | diagrama da troca de mensagens  |
| Desenvolvimento             | Gabriel, Pedro Salles, Matheus | código no repositório git       |
| Revisão de código           | Renan, Augusto                 | registro das revisões           |
| Testes e desempenho         | João Pedro Tomás, José Neto    | tabela de testes e gráficos S(P)|
| Documentação e apresentação | João Pedro                     | slides e apresentação           |

**Regra:** quem desenvolve um trecho não revisa esse trecho.

## Cronograma (por atividades)

**Prazo final:** tudo pronto antes do seminário, terça 17/11/2026.

| #  | Atividade                              | Responsável            | Depende de | Status       |
|----|----------------------------------------|------------------------|------------|--------------|
| 1  | Plano e papéis                         | Gabriel                | —          | concluído    |
| 2  | Versão serial (linha de base T1)       | devs                   | 1          | a fazer      |
| 3  | Diagrama da troca de mensagens         | Gabriel                | 1          | a fazer      |
| 4  | Versão MPI                             | devs                   | 2, 3       | a fazer      |
| 5  | Revisão do código                      | Renan, Augusto         | 2, 4       | a fazer      |
| 6  | Testes de corretude                    | testes                 | 2, 4       | a fazer      |
| 7  | Correções apontadas em 5 e 6           | devs                   | 5, 6       | a fazer      |
| 8  | Benchmark e gráficos de S(P)           | testes                 | 7          | a fazer      |
| 9  | Slides                                 | João Pedro             | 3, 6, 8    | a fazer      |
| 10 | Ensaio da apresentação                 | todos                  | 9          | a fazer      |

## Tarefas

| Tarefa                                         | Responsável       | Revisor  |
|------------------------------------------------|-------------------|----------|
| Versão serial (adaptar `zpipe.c` para gzip)    | Pedro Salles      | Augusto  |
| Comunicação MPI (divisão e junção dos blocos)  | Gabriel           | Renan    |
| Compressão por bloco e formato gzip (CRC, trailer) | Matheus       | Augusto  |
| Diagrama da troca de mensagens                 | Gabriel           | Renan    |
| Testes de corretude (`gzip -t` e `cmp`)        | João Pedro Tomás  | —        |
| Benchmark e gráficos de S(P)                   | José Neto         | —        |
| Slides e roteiro da apresentação               | João Pedro        | todos    |