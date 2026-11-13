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
- [ ] Toda coletiva (`Bcast`, `Scatterv`, `Gather`, `Gatherv`) é chamada por **todos** os ranks, na **mesma ordem**?
- [ ] Os tamanhos de envio e de recepção casam? (o que o rank i manda = o que o rank 0 espera dele)
- [ ] Quando o arquivo acaba (`n == 0`), todos saem do laço juntos?
- [ ] Em erro, usa `MPI_Abort` (e não `return`/`exit` num rank só)?
- [ ] Os CRCs são juntados **em ordem** e com o tamanho **original** de cada bloco?
- [ ] As contagens são `int`: `P × tamanho do bloco` cabe sem estourar?
- [ ] O código bate com o diagrama em `docs/ARQUITETURA.md`?

### Augusto — zlib e C (`zpipe_serial.c`, `tempo.c`, `bloco.c`)
- [ ] O retorno de `deflate`, `fwrite`, `fclose` e `malloc` é sempre conferido?
- [ ] O buffer de saída tem espaço suficiente (`deflateBound` + folga)?
- [ ] O cabeçalho e o fim do gzip seguem o formato (RFC 1952)?
- [ ] A serial e a MPI medem o tempo no **mesmo intervalo**?
- [ ] Tem vazamento de memória ou arquivo aberto sem fechar?

## Registro

### Renan

| # | Data | Arquivo:linha | O que foi apontado | Como foi resolvido | Commit |
|---|------|---------------|--------------------|--------------------|--------|
| R1 |     |               |                    |                    |        |

### Augusto

| # | Data | Arquivo:linha | O que foi apontado | Como foi resolvido | Commit |
|---|------|---------------|--------------------|--------------------|--------|
| A1 |     |               |                    |                    |        |

## Situação final
- `docs/ARQUITETURA.md`: ☐ aprovado (Renan)
- `src/pgz_mpi.c`: ☐ aprovado (Renan)
- `src/zpipe_serial.c` / `src/tempo.c`: ☐ aprovado (Augusto)
- `src/bloco.c`: ☐ aprovado (Augusto)
