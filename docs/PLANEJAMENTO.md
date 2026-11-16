# Planejamento — zlib MPI (Seminário GSI059)

**Disciplina:** Programação Paralela — prof. Gilberto Nakamura
**Tema:** paralelizar com MPI a compressão gzip da zlib (`examples/zpipe.c`)
**Gestor:** Gabriel
**Última atualização:** 16/11/2026

> Documento vivo: atualizado pelo gestor ao longo da semana.

## Objetivo
Comprimir um arquivo em blocos, em paralelo com MPI, gerando um `.gz` válido
(que o `gunzip` comum abra), e medir o ganho de desempenho em relação à
versão serial: S(P) = T1 / TP.

## Papéis

O grupo tem 8 integrantes (menos que os 10 previstos); por isso Gabriel
acumula gestão, arquitetura e parte do desenvolvimento.

| Segmento                    | Integrante(s)                  | Entregável                      |
|-----------------------------|--------------------------------|---------------------------------|
| Gestão e planejamento       | Gabriel                        | este plano de uma página        |
| Arquitetura paralela        | Gabriel                        | diagrama da troca de mensagens  |
| Desenvolvimento             | Gabriel, Pedro Salles, Matheus | código no repositório git       |
| Revisão de código           | Renan, Augusto                 | registro das revisões           |
| Testes e desempenho         | João Pedro Tomaz, José Neto    | tabela de testes e gráficos S(P)|
| Documentação e apresentação | João Pedro                     | slides e apresentação           |

**Regra:** quem desenvolve um trecho não revisa esse trecho.

## Cronograma (por atividades)

**Prazo final:** tudo pronto antes do seminário, terça 17/11/2026.

| #  | Atividade                              | Responsável            | Depende de | Status                         |
|----|----------------------------------------|------------------------|------------|--------------------------------|
| 1  | Plano e papéis                         | Gabriel                | —          | concluído                      |
| 2  | Versão serial (linha de base T1)       | Pedro Salles           | 1          | concluído                      |
| 3  | Diagrama da troca de mensagens         | Gabriel                | 1          | concluído                      |
| 4  | Versão MPI                             | Gabriel, Matheus       | 2, 3       | concluído                      |
| 5  | Revisão do código                      | Renan, Augusto         | 2, 4       | a fazer (`docs/REVISOES.md`)   |
| 6  | Testes de corretude                    | João Pedro Tomaz       | 2, 4       | scripts no repo; 30/30 passando; falta preencher `docs/TESTES.md` |
| 7  | Correções apontadas em 5 e 6           | devs                   | 5, 6       | a fazer                        |
| 8  | Benchmark e gráficos de S(P)           | José Neto              | 7          | scripts no repo; falta rodada final e análise em `docs/TESTES.md` |
| 9  | Slides                                 | João Pedro             | 3, 6, 8    | prévia pronta; falta versão final em `slides/` |
| 10 | Ensaio da apresentação                 | todos                  | 9          | a fazer                        |

## Tarefas

| Tarefa                                         | Arquivos                          | Responsável       | Revisor  |
|------------------------------------------------|-----------------------------------|-------------------|----------|
| Versão serial e medição de tempo               | `src/zpipe_serial.c`, `src/tempo.*` | Pedro Salles    | Augusto  |
| Comunicação MPI (divisão e junção dos blocos)  | `src/pgz_mpi.c`                   | Gabriel           | Renan    |
| Compressão por bloco e formato gzip            | `src/bloco.*`                     | Matheus           | Augusto  |
| Diagrama da troca de mensagens                 | `docs/ARQUITETURA.md`             | Gabriel           | Renan    |
| Testes de corretude (`gzip -t` e `cmp`)        | `tests/`, `docs/TESTES.md`        | João Pedro Tomaz  | —        |
| Benchmark e gráficos de S(P)                   | `bench/`, `docs/TESTES.md`        | José Neto         | —        |
| Revisão de código                              | `docs/REVISOES.md`                | Renan, Augusto    | —        |
| Slides e roteiro da apresentação               | `slides/`                         | João Pedro        | todos    |

## Andamento

- **12/11:** plano, papéis, cronograma e arquitetura concluídos.
- **13/11:** versão MPI concluída e testada localmente (30/30 testes de
  corretude passando; S(2) ≈ 1,7 com 128 MB). Estrutura do repositório
  criada; instruções enviadas a cada integrante; modelos de revisão e de
  relatório de testes no repositório.
- **16/11:** Pedro (serial e tempo) e José Neto (scripts de benchmark)
  subiram suas partes. Por causa do prazo, o gestor subiu `src/bloco.*`
  (Matheus) e `tests/*.sh` (João Pedro Tomaz). A versão do GitHub compila
  sem avisos e passa nos 30 testes de corretude. Prévia dos slides pronta.
- **Próximo:** revisões (Renan, Augusto) → correções → benchmark final e
  `docs/TESTES.md` → slides finais → ensaio.