#!/usr/bin/env bash
# gen_data.sh — gera os arquivos de teste em data/ (não vão para o GitHub).
# Responsável: João Pedro Tomás
#
# Uso: bash tests/gen_data.sh          (grandes com 128 MB)
#      BIG_MB=32 bash tests/gen_data.sh (grandes menores, para testar rápido)
set -eu
cd "$(dirname "$0")/.."              # vai para a raiz do projeto
mkdir -p data
BIG_MB=${BIG_MB:-128}
MB=$((1024 * 1024))

# --- casos de borda (corretude) ---
: > data/vazio.bin                                        # 0 bytes
printf 'x' > data/um_byte.bin                             # 1 byte
seq 1 20000 > data/texto_pequeno.txt                      # menor que 1 bloco
head -c $MB /dev/urandom > data/bloco_exato.bin           # exatamente 1 bloco
head -c $((MB + 1)) /dev/urandom > data/bloco_mais_um.bin # 1 bloco + 1 byte
head -c $((3 * MB - 7)) /dev/urandom | base64 > data/base64_medio.txt  # várias rodadas

# --- arquivos grandes (desempenho); só cria se ainda não existir ---
[ -f data/zeros_big.bin ]  || head -c $((BIG_MB * MB)) /dev/zero    > data/zeros_big.bin
[ -f data/random_big.bin ] || head -c $((BIG_MB * MB)) /dev/urandom > data/random_big.bin
[ -f data/texto_big.txt ]  || { seq 1 100000000 | head -c $((BIG_MB * MB)) || true; } > data/texto_big.txt

ls -l data