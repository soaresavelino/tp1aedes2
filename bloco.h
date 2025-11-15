#ifndef BLOCO_H
#define BLOCO_H

typedef struct {
    int id;
    int bytesUsados;
    int tamanhoBloco;
    char *dados;
} Bloco;

Bloco* criarBloco(int id, int tamanho);
void liberarBloco(Bloco *b);
int adicionarRegistro(Bloco *b, const char *registro, int tamanho, FILE *arquivo, int modoEspalhado, int idBloco);
void gravarBloco(FILE *arquivo, Bloco *b);
void exibirMapaBlocos(int totalBlocos, int *ocupacao, int tamanhoBloco);

#endif
