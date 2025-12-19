#include "gerador.h"
#include <stdlib.h>
#include "gerador.h"
#include "salvar_original.h"


Aluno *gerarListaAlunos(int n) {
    Aluno *lista = malloc(sizeof(Aluno) * n);
    for (int i = 0; i < n; i++) {
        lista[i] = gerarAlunoFicticio(i + 1);
    }
    return lista;
}

// Salva o arquivo original exatamente como está na memória (BINARIO).