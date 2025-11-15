#ifndef ALUNO_H
#define ALUNO_H

typedef struct {
    int matricula;
    char nome[50];
    char cpf[12];
    char curso[30];
    char mae[50];
    char pai[50];
    int anoIngresso;
    float ca;
    int removido;  // 0 = ativo, 1 = removido
} Aluno;


// Funções
Aluno gerarAlunoFicticio(int id);
int tamanhoRegistroFixo();
int tamanhoRegistroVariavel(const Aluno *a);
void serializarAlunoFixo(const Aluno *a, char *buffer);
void serializarAlunoVariavel(const Aluno *a, char *buffer);
void deserializarAlunoFixo(const char *buffer, Aluno *a);
void deserializarAlunoVariavel(const char *buffer, Aluno *a);
void listarRegistros();
void listarRegistrosVariaveis(const char *nomeArquivo, int tamanhoBloco);
void adicionarRegistroArquivo(const Aluno *a, int tipo, int tamanhoBloco, int espalhado);

void excluirRegistroArquivo(int matricula, int tipo, int tamanhoBloco);

void editarRegistroArquivo(int matricula, int tipo, int tamanhoBloco);



#endif
