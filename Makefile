# Compilar dentro do Ubuntu (WSL):  make
CC     = gcc
MPICC  = mpicc
CFLAGS = -O2 -Wall -Wextra -std=c11
LIBS   = -lz

all: zpipe_serial pgz_mpi

# versão serial (Pedro)
zpipe_serial: src/zpipe_serial.c src/tempo.c src/tempo.h
	$(CC) $(CFLAGS) -o zpipe_serial src/zpipe_serial.c src/tempo.c $(LIBS)

# versão MPI (Gabriel + Matheus)
pgz_mpi: src/pgz_mpi.c src/bloco.c src/bloco.h src/tempo.c src/tempo.h
	$(MPICC) $(CFLAGS) -o pgz_mpi src/pgz_mpi.c src/bloco.c src/tempo.c $(LIBS)

clean:
	rm -f zpipe_serial pgz_mpi *.gz

.PHONY: all clean