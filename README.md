# zlib-mpi — compressão gzip em blocos com MPI

Seminário de Programação Paralela (GSI059) — prof. Gilberto Nakamura.

Paraleliza com MPI a compressão do exemplo oficial da zlib
(`examples/zpipe.c`), gerando um `.gz` válido, e compara o desempenho
com a versão serial: S(P) = T1 / TP.

## Estrutura e responsáveis

| Arquivo                          | Responsável       | Revisor  |
|----------------------------------|-------------------|----------|
| `docs/PLANEJAMENTO.md`           | Gabriel           | —        |
| `docs/ARQUITETURA.md`            | Gabriel           | Renan    |
| `src/zpipe_serial.c`             | Pedro Salles      | Augusto  |
| `src/pgz_mpi.c`                  | Gabriel           | Renan    |
| `src/bloco.c`, `src/bloco.h`     | Matheus           | Augusto  |
| `tests/gen_data.sh`, `tests/run_tests.sh` | João Pedro Tomás | — |
| `bench/run_bench.sh`, `bench/plot.py`     | José Neto        | — |
| `docs/TESTES.md`                 | João Pedro Tomás, José Neto | — |
| `docs/REVISOES.md`               | Renan, Augusto    | —        |
| `slides/`                        | João Pedro        | todos    |

## Como colaborar
1. Clone o repositório:
   `git clone https://github.com/Gab-Andrade/zlib-mpi-Paraleliza-o`
2. Edite **só os seus arquivos** (tabela acima).
3. Faça o commit e o push:
   `git add <arquivo>` → `git commit -m "mensagem"` → `git push`

## Como compilar (Ubuntu/WSL)
    sudo apt install build-essential zlib1g-dev openmpi-bin libopenmpi-dev
    make

## Declaração de uso de IA
Parte do código e dos documentos foi produzida com auxílio do Claude
(Anthropic), usado como tutor. Todo o código foi lido, testado e revisado
pelos integrantes do grupo; as revisões estão em `docs/REVISOES.md`.