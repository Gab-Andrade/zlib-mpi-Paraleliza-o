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

## Cronograma
_(a definir)_

## Tarefas
_(a definir)_