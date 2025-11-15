#include <stdio.h>
#include "aluno.h"

#include "salvar_original.h"

// Salva o arquivo original exatamente como está na memória (BINARIO)
void salvarOriginal(Aluno *alunos, int n) {
    FILE *f = fopen("alunos_original.dat", "wb");
    if (!f) {
        printf("Erro ao criar alunos_original.dat\n");
        return;
    }

    for (int i = 0; i < n; i++) {
        fwrite(&alunos[i], sizeof(Aluno), 1, f);
    }

    fclose(f);
}
