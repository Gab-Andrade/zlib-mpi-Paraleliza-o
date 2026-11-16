#!/usr/bin/env bash
# run_bench.sh - mede os tempos da versão serial e da MPI
# Responsável: José Neto
#
# Uso: bash bench/run_bench.sh   (REP=1 para um teste rápido)
set -eu
cd "$(dirname "$0")/.."

REP=${REP:-5}
PS="1 2 3 4 5 6 7 8 12 16"
ARQS="texto_big.txt random_big.bin zeros_big.bin"
WORK=$HOME/pgz_bench   # fora do /mnt/c, que é lento

mkdir -p "$WORK" results
[ -f data/texto_big.txt ] || bash tests/gen_data.sh > /dev/null
make -s
cp zpipe_serial pgz_mpi "$WORK"/

out=results/bench.csv
echo "arquivo,rep,versao,P,bytes_in,bytes_out,tempo" > "$out"

for a in $ARQS; do
    cp "data/$a" "$WORK/"
    for r in $(seq 1 "$REP"); do
        linha=$("$WORK/zpipe_serial" "$WORK/$a" "$WORK/s.gz")
        echo "$a,$r,$linha" >> "$out"
        for p in $PS; do
            linha=$(mpirun --use-hwthread-cpus --bind-to none -np "$p" "$WORK/pgz_mpi" "$WORK/$a" "$WORK/m.gz")
            echo "$a,$r,$linha" >> "$out"
        done
    done
    rm -f "$WORK/$a"
    echo "terminou $a"
done

rm -rf "$WORK"
echo "resultados em $out"
