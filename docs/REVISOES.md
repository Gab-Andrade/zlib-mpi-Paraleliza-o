# Registro de revisões

**Revisores:** Renan e Augusto
**Regra:** quem desenvolve um trecho não revisa esse trecho.

| Arquivo                          | Autor            | Revisor  |
|----------------------------------|------------------|----------|
| `docs/ARQUITETURA.md`            | Gabriel          | Renan    |
| `src/pgz_mpi.c`                  | Gabriel          | Renan    |
| `src/zpipe_serial.c`, `src/tempo.c`, `src/tempo.h` | Pedro Salles | Augusto |
| `src/bloco.c`, `src/bloco.h`     | Matheus          | Augusto  |

## O que conferir


### Renan — comunicação MPI (`pgz_mpi.c`)
- [x] Toda coletiva (`Bcast`, `Scatterv`, `Gather`, `Gatherv`) é chamada por **todos** os ranks, na **mesma ordem**?
- [x] Os tamanhos de envio e de recepção casam? (o que o rank i manda = o que o rank 0 espera dele)
- [x] Quando o arquivo acaba (`n == 0`), todos saem do laço juntos?
- [x] Em erro, usa `MPI_Abort` (e não `return`/`exit` num rank só)?
- [x] Os CRCs são juntados **em ordem** e com o tamanho **original** de cada bloco?
- [x] As contagens são `int`: `P × tamanho do bloco` cabe sem estourar no escopo previsto?
- [x] O código bate com o diagrama em `docs/ARQUITETURA.md`?

### Augusto — zlib e C (`zpipe_serial.c`, `tempo.c`, `bloco.c`)
- [x] O retorno de `deflate`, `fwrite`, `fclose` e `malloc` é sempre conferido?
- [x] O buffer de saída tem espaço suficiente (`deflateBound` + folga)?
- [x] O cabeçalho e o fim do gzip seguem o formato (RFC 1952)?
- [x] A serial e a MPI medem o tempo no **mesmo intervalo**?
- [x] Tem vazamento de memória ou arquivo aberto sem fechar?

## Registro


### Renan

| # | Data | Arquivo:linha | O que foi apontado | Como foi resolvido | Commit |
|---|------|---------------|--------------------|--------------------|--------|
| — | 16/11/2026 | `src/pgz_mpi.c` e `docs/ARQUITETURA.md` | Revisão concluída. Nenhuma correção necessária dentro do escopo definido para o projeto. | Código e documentação aprovados após análise e testes. | — |


### Augusto

| # | Data | Arquivo:linha | O que foi apontado | Como foi resolvido | Commit |
|---|------|---------------|--------------------|--------------------|--------|
| A1 | 17/11/2026 | `src/zpipe_serial.c:53-57` | Os dois `fopen` são feitos antes de conferir: se a entrada não existe, a saída é criada vazia (0 bytes) e o arquivo que abriu fica aberto. | Aceito sem correção: só acontece em uso errado, não afeta a saída nem o tempo medido. | — |
| A2 | 17/11/2026 | `src/zpipe_serial.c:63-67` | Se `deflateInit2` falha, sai sem fechar `src` e `dst`. | Aceito sem correção: o processo termina em seguida e o sistema fecha os arquivos. | — |
| A3 | 17/11/2026 | `src/bloco.c:37` | Retorno de `deflateReset` não é conferido. | Aceito sem correção: só falharia com o `z_stream` inválido, e o `bloco_inicia` já confere a inicialização. | — |
| A4 | 17/11/2026 | `src/pgz_mpi.c:64` × `src/zpipe_serial.c:70` | A MPI grava o cabeçalho (10 bytes) antes de iniciar o cronômetro; a serial grava dentro do intervalo. | Só observação: diferença desprezível, sem correção. | — |
| — | 17/11/2026 | testes | Serial e `bloco.c` testados com texto (`seq 1 1000000`), 5 MB aleatórios e arquivo vazio: `gzip -t` e `cmp` OK em todos. | — | — |

## Situação final

- `docs/ARQUITETURA.md`: ☑ aprovado (Renan)
- `src/pgz_mpi.c`: ☑ aprovado (Renan)
- `src/zpipe_serial.c` / `src/tempo.c`: ☑ aprovado (Augusto)
- `src/bloco.c`: ☑ aprovado (Augusto)
