# Testes e desempenho

**Responsáveis:** João Pedro Tomás (corretude) e José Neto (desempenho)

## Ambiente
- Processador: _(ex.: Intel i3-1115G4, 2 núcleos físicos, 4 threads)_
- Sistema: _(ex.: Ubuntu 24.04 no WSL)_
- Compilador / MPI / zlib: _(saída de `gcc --version` e `mpirun --version`)_

## 1. Corretude (João Pedro Tomás)

Critério do professor: o `.gz` passa em `gzip -t` **e** na ida e volta
byte a byte (`gunzip -c saida.gz | cmp - entrada`).

Como rodar: `bash tests/gen_data.sh && bash tests/run_tests.sh`

| Arquivo | Tamanho | Por que testar | Serial | P=1 | P=2 | P=3 | P=4 |
|---|---|---|---|---|---|---|---|
| `vazio.bin` | 0 B | arquivo vazio | | | | | |
| `um_byte.bin` | 1 B | menor caso possível | | | | | |
| `texto_pequeno.txt` | ~100 KB | menor que um bloco | | | | | |
| `bloco_exato.bin` | 1 MB | exatamente 1 bloco | | | | | |
| `bloco_mais_um.bin` | 1 MB + 1 B | sobra 1 byte | | | | | |
| `base64_medio.txt` | ~4 MB | várias rodadas | | | | | |

Resultado: _( __ de 30 testes passaram )_ — tabela completa em `results/tests.csv`.

## 2. Desempenho (José Neto)

S(P) = T1 / TP, com **T1 = versão serial**. Mediana de 5 repetições,
arquivos de 128 MB copiados para o disco do Linux antes de medir.

Como rodar: `bash bench/run_bench.sh && python3 bench/plot.py`

_(colar aqui a tabela de `results/desempenho.md`)_

![Speedup](../results/speedup.png)

## 3. Análise (José Neto)
- Com P = 2: _(quanto acelerou e por que não chegou a 2×)_
- Com P = 3 e 4: _(o que aconteceu e por quê — núcleos físicos × hyperthreading)_
- Texto × aleatório × zeros: _(por que cada um acelerou diferente)_
- Limitação da arquitetura: _(o rank 0 lê, junta e grava sozinho)_
