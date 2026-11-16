#!/usr/bin/env bash
# run_tests.sh — testes de corretude da serial e da versão MPI.
# Responsável: João Pedro Tomás
#
# Para cada arquivo de data/ (menos os _big) e cada P:
#   1) gzip -t aceita o .gz?   2) gunzip devolve exatamente a entrada?
# Resultado: tabela na tela e em results/tests.csv
set -u
cd "$(dirname "$0")/.."
mkdir -p results
[ -f data/vazio.bin ] || bash tests/gen_data.sh > /dev/null

PS="1 2 3 4"
TMP=$(mktemp -d)
trap 'rm -rf "$TMP"' EXIT

out=results/tests.csv
echo "entrada,bytes,versao,P,gzip_t,ida_e_volta,resultado" > "$out"
falhas=0

# confere um .gz contra o original e grava uma linha na tabela
confere() {   # $1=arquivo original  $2=versao  $3=P
    local t c r
    gzip -t "$TMP/x.gz" 2>/dev/null && t=ok || t=ERRO
    gunzip -c "$TMP/x.gz" 2>/dev/null | cmp -s - "$1" && c=ok || c=ERRO
    if [ "$t" = ok ] && [ "$c" = ok ]; then r=PASSOU; else r=FALHOU; falhas=$((falhas + 1)); fi
    echo "$(basename "$1"),$(stat -c %s "$1"),$2,$3,$t,$c,$r" >> "$out"
}

for f in data/*; do
    case "$f" in *_big*) continue ;; esac          # grandes ficam para o benchmark

    ./zpipe_serial "$f" "$TMP/x.gz" > /dev/null
    confere "$f" serial 1

    for p in $PS; do
        mpirun --oversubscribe -np "$p" ./pgz_mpi "$f" "$TMP/x.gz" > /dev/null
        confere "$f" mpi "$p"
    done
done

column -s, -t "$out"
echo
if [ $falhas -eq 0 ]; then echo "TODOS OS TESTES PASSARAM"; else echo "$falhas TESTE(S) FALHARAM"; fi
exit $falhas