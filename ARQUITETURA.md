# Arquitetura paralela — troca de mensagens

**Responsável:** Gabriel · **Revisor:** Renan

Rank 0 lê, distribui, recebe e grava. Todos os ranks comprimem.
O arquivo é processado em rodadas de P blocos (1 bloco = 1 MB).

```mermaid
sequenceDiagram
    participant R0 as Rank 0
    participant Ri as Ranks 1..P-1

    Note over R0: grava cabeçalho gzip
    loop cada rodada
        Note over R0: lê até P blocos do arquivo
        R0->>Ri: quantos bytes foram lidos (0 = acabou)
        R0->>Ri: bloco i para o rank i
        Note over R0,Ri: cada um comprime seu bloco e calcula o CRC
        Ri->>R0: tamanho comprimido + CRC
        Ri->>R0: bytes comprimidos (chegam em ordem)
        Note over R0: grava os blocos e junta os CRCs em ordem
    end
    Note over R0: grava o fim do gzip (CRC total + tamanho)
```