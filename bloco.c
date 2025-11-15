#include "bloco.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Bloco* criarBloco(int id, int tamanho) {
    Bloco *b = malloc(sizeof(Bloco));
    b->id = id;
    b->bytesUsados = 0;
    b->tamanhoBloco = tamanho;
    b->dados = calloc(tamanho, 1);
    return b;
}

void liberarBloco(Bloco *b) {
    free(b->dados);
    free(b);
}

void gravarBloco(FILE *arquivo, Bloco *b) {
    fwrite(b->dados, 1, b->bytesUsados, arquivo);
}

int adicionarRegistro(Bloco *b, const char *registro, int tamanho, FILE *arquivo, int modoEspalhado, int idBloco) {
    // Se não cabe no bloco, grava e inicia novo bloco
    if (b->bytesUsados + tamanho > b->tamanhoBloco) {
        gravarBloco(arquivo, b);     // escreve bloco atual no arquivo
        b->bytesUsados = 0;          // reinicia ocupação
        memset(b->dados, 0, b->tamanhoBloco);
    }

    // Copia registro no bloco
    memcpy(b->dados + b->bytesUsados, registro, tamanho);
    b->bytesUsados += tamanho;

    return (b->bytesUsados == tamanho) ? 1 : 0; // retorna 1 se criou novo bloco
}


// Exibe mapa de ocupação aprimorado
void exibirMapaBlocos(int totalBlocos, int *ocupacao, int tamanhoBloco) {
    int blocosParcialmenteUsados = 0;
    int totalBytesUsados = 0;
    int totalBytesDisponiveis = totalBlocos * tamanhoBloco;

    printf("\n===== Mapa de Ocupacao =====\n");

    for (int i = 0; i < totalBlocos; i++) {
        // Calcula quantos blocos reais este "bloco lógico" ocupa
        float percentual = ((float)ocupacao[i] / tamanhoBloco) * 100.0;
        if (percentual > 100.0) percentual = 100.0; // limitar a 100%
        
        printf("Bloco %d: %d bytes (%.1f%% cheio)\n", i + 1, ocupacao[i], percentual);

        totalBytesUsados += ocupacao[i];
        if (ocupacao[i] > 0 && ocupacao[i] < tamanhoBloco) {
            blocosParcialmenteUsados++;
        }
    }

    float ocupacaoMedia = ((float)totalBytesUsados / (totalBlocos * tamanhoBloco)) * 100.0;
    if (ocupacaoMedia > 100.0) ocupacaoMedia = 100.0;

    printf("\nTotal de blocos: %d\n", totalBlocos);
    printf("Blocos parcialmente usados: %d\n", blocosParcialmenteUsados);
    printf("Ocupacao media: %.1f%%\n", ocupacaoMedia);
    printf("Eficiencia total: %.1f%%\n", ocupacaoMedia);

    if (blocosParcialmenteUsados > 0) {
        printf("\nSugestao: considerar reorganizacao fisica para reduzir desperdicio de espaco.\n");
    }
}