#!/usr/bin/env bash
# run_bench.sh — mede os tempos da serial e da versão MPI.
# Responsável: José Neto
#
# Uso: bash bench/run_bench.sh          (5 repetições)
#      REP=1 bash bench/run_bench.sh    (teste rápido)
# Resultado bruto: results/bench.csv  (os gráficos saem do bench/plot.py)
set -eu
cd "$(dirname "$0")/.."

REP=${REP:-5}
PS="1 2 3 4 5 6 7 8 12 16"      # 8 núcleos físicos; 12 e 16 já usam o SMT (2 threads por núcleo)
ARQS="texto_big.txt random_big.bin zeros_big.bin"
WORK=$HOME/pgz_bench            # disco do Linux: /mnt/c é lento e distorce a medida

mkdir -p "$WORK" results
[ -f data/texto_big.txt ] || bash tests/gen_data.sh > /dev/null
make -s
cp zpipe_serial pgz_mpi "$WORK"/

out=results/bench.csv
echo "arquivo,rep,versao,P,bytes_in,bytes_out,tempo" > "$out"

for a in $ARQS; do
    cp "data/$a" "$WORK/"                       # copia a entrada para o disco do Linux
    for r in $(seq 1 "$REP"); do
        linha=$("$WORK/zpipe_serial" "$WORK/$a" "$WORK/s.gz")
        echo "$a,$r,$linha" >> "$out"
        for p in $PS; do
            # --use-hwthread-cpus: 16 slots (8 núcleos x 2 threads)
            # --bind-to none: o Linux espalha os ranks pelos núcleos físicos antes de usar o SMT
            linha=$(mpirun --use-hwthread-cpus --bind-to none -np "$p" "$WORK/pgz_mpi" "$WORK/$a" "$WORK/m.gz")
            echo "$a,$r,$linha" >> "$out"
        done
    done
    rm -f "$WORK/$a"
    echo "terminou $a"
done

rm -rf "$WORK"
echo "resultados em $out"
