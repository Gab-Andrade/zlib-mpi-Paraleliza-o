"""plot.py — tabela e gráfico de speedup a partir de results/bench.csv.
Responsável: José Neto

S(P) = T1 / TP, com T1 = mediana da versão SERIAL (não do MPI com P=1).
E(P) = S(P) / P  (eficiência: 1,0 = aproveitou 100% dos processos)
"""
import csv
from collections import defaultdict
from statistics import median

import matplotlib
matplotlib.use("Agg")                     # gera a imagem sem abrir janela
import matplotlib.pyplot as plt

NUCLEOS = 8                               # núcleos físicos do PC do benchmark (Ryzen 7 5700X3D)

# 1. Lê os tempos e agrupa por (arquivo, versao, P)
tempos = defaultdict(list)
with open("results/bench.csv") as f:
    for lin in csv.DictReader(f):
        tempos[(lin["arquivo"], lin["versao"], int(lin["P"]))].append(float(lin["tempo"]))

arquivos = sorted({a for a, _, _ in tempos})
Ps = sorted({p for a, v, p in tempos if v == "mpi"})

# 2. Tabela em Markdown
linhas = ["| arquivo | P | T (s, mediana) | S(P) | E(P) |", "|---|---|---|---|---|"]
for a in arquivos:
    t1 = median(tempos[(a, "serial", 1)])
    linhas.append(f"| {a} | serial | {t1:.3f} | 1.00 | — |")
    for p in Ps:
        tp = median(tempos[(a, "mpi", p)])
        s = t1 / tp
        linhas.append(f"| {a} | {p} | {tp:.3f} | {s:.2f} | {s / p:.2f} |")

with open("results/desempenho.md", "w") as f:
    f.write("\n".join(linhas) + "\n")
print("\n".join(linhas))

# 3. Gráfico de S(P)
plt.figure(figsize=(6, 4))
plt.plot(Ps, Ps, "k--", label="ideal (S = P)")
for a in arquivos:
    t1 = median(tempos[(a, "serial", 1)])
    plt.plot(Ps, [t1 / median(tempos[(a, "mpi", p)]) for p in Ps], "o-", label=a)
if max(Ps) > NUCLEOS:                     # daqui em diante, 2 processos dividem um núcleo
    plt.axvline(NUCLEOS + 0.5, color="gray", alpha=0.3)
    plt.text(NUCLEOS + 0.6, max(Ps) * 0.9, f"acima de {NUCLEOS}: SMT/hyperthreading",
             fontsize=8, color="gray")
plt.xlabel("P (processos)")
plt.ylabel("S(P) = T1 / TP")
plt.xticks(Ps)
plt.title("Speedup da compressão gzip com MPI")
plt.legend()
plt.grid(alpha=0.3)
plt.tight_layout()
plt.savefig("results/speedup.png", dpi=150)
print("gráfico em results/speedup.png")
