#include "aluno.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Gera aluno fictício
Aluno gerarAlunoFicticio(int id) {
    Aluno a;
    a.matricula = 100000000 + id;
    sprintf(a.nome, "Aluno_%d", id);
    sprintf(a.cpf, "000000%05d", id);
    sprintf(a.curso, "Curso_%d", id % 5);
    sprintf(a.mae, "Mae_%d", id);
    sprintf(a.pai, "Pai_%d", id);
    a.anoIngresso = 2015 + (id % 10);
    a.ca = (float)(id % 100) / 10.0;
    a.removido = 0; // ativo
    return a;
}

// Tamanho fixo do registro
int tamanhoRegistroFixo() {
    return sizeof(int) + 51 + 12 + 31 + 31 + 31 + sizeof(int) + sizeof(float) + sizeof(int);
}

// Tamanho variável do registro (sem padding)
int tamanhoRegistroVariavel(const Aluno *a) {
    return sizeof(int) + // matricula
           sizeof(int) + // removido
           strlen(a->nome) + 1 +
           strlen(a->cpf) + 1 +
           strlen(a->curso) + 1 +
           strlen(a->mae) + 1 +
           strlen(a->pai) + 1 +
           sizeof(int) + sizeof(float);
}

// Serializa aluno fixo
void serializarAlunoFixo(const Aluno *a, char *buffer) {
    int pos = 0;
    memcpy(buffer + pos, &a->matricula, sizeof(int)); pos += sizeof(int);

    char tmp[60];
    strcpy(tmp, a->nome); padString(tmp, 50);
    memcpy(buffer + pos, tmp, 51); pos += 51;

    strcpy(tmp, a->cpf); padString(tmp, 11);
    memcpy(buffer + pos, tmp, 12); pos += 12;

    strcpy(tmp, a->curso); padString(tmp, 30);
    memcpy(buffer + pos, tmp, 31); pos += 31;

    strcpy(tmp, a->mae); padString(tmp, 30);
    memcpy(buffer + pos, tmp, 31); pos += 31;

    strcpy(tmp, a->pai); padString(tmp, 30);
    memcpy(buffer + pos, tmp, 31); pos += 31;

    memcpy(buffer + pos, &a->anoIngresso, sizeof(int)); pos += sizeof(int);
    memcpy(buffer + pos, &a->ca, sizeof(float)); pos += sizeof(float);

    memcpy(buffer + pos, &a->removido, sizeof(int)); // flag de removido
}

// Serializa aluno variável
void serializarAlunoVariavel(const Aluno *a, char *buffer) {
    int pos = 0;
    memcpy(buffer + pos, &a->matricula, sizeof(int)); pos += sizeof(int);
    memcpy(buffer + pos, &a->removido, sizeof(int)); pos += sizeof(int);
    sprintf(buffer + pos, "%s|%s|%s|%s|%s|%d|%.2f",
            a->nome, a->cpf, a->curso, a->mae, a->pai, a->anoIngresso, a->ca);
}

// Deserializa fixo
void deserializarAlunoFixo(const char *buffer, Aluno *a) {
    int pos = 0;
    memcpy(&a->matricula, buffer + pos, sizeof(int)); pos += sizeof(int);
    memcpy(a->nome, buffer + pos, 51); a->nome[50]='\0'; pos+=51;
    memcpy(a->cpf, buffer + pos, 12); a->cpf[11]='\0'; pos+=12;
    memcpy(a->curso, buffer + pos, 31); a->curso[30]='\0'; pos+=31;
    memcpy(a->mae, buffer + pos, 31); a->mae[30]='\0'; pos+=31;
    memcpy(a->pai, buffer + pos, 31); a->pai[30]='\0'; pos+=31;
    memcpy(&a->anoIngresso, buffer + pos, sizeof(int)); pos+=sizeof(int);
    memcpy(&a->ca, buffer + pos, sizeof(float)); pos+=sizeof(float);
    memcpy(&a->removido, buffer + pos, sizeof(int));
}

// Deserializa variável
void deserializarAlunoVariavel(const char *buffer, Aluno *a) {
    int pos = 0;
    memcpy(&a->matricula, buffer + pos, sizeof(int)); pos += sizeof(int);
    memcpy(&a->removido, buffer + pos, sizeof(int)); pos += sizeof(int);

    char resto[400];
    strcpy(resto, buffer + pos);
    char *token = strtok(resto, "|");
    strcpy(a->nome, token);
    token = strtok(NULL, "|"); strcpy(a->cpf, token);
    token = strtok(NULL, "|"); strcpy(a->curso, token);
    token = strtok(NULL, "|"); strcpy(a->mae, token);
    token = strtok(NULL, "|"); strcpy(a->pai, token);
    token = strtok(NULL, "|"); a->anoIngresso = atoi(token);
    token = strtok(NULL, "|"); a->ca = atof(token);
}



// Lista registros
void listarRegistros() {
    FILE *f = fopen("alunos.dat", "rb");
    if (!f) {
        printf("Arquivo inexistente.\n");
        return;
    }

    int tipo;
    if (fread(&tipo, sizeof(int), 1, f) != 1) { 
        fclose(f); 
        return; 
    }

    printf("\n=== LISTAGEM DE REGISTROS ===\n");

    if (tipo == 1) { // registros fixos
        int tam = tamanhoRegistroFixo();
        char *buffer = malloc(tam);

        while (fread(buffer, tam, 1, f) == 1) {
            Aluno a;
            deserializarAlunoFixo(buffer, &a);
            if (!a.removido) {
                printf("----------------------------\n");
                printf("Matricula: %d\nNome: %s\nCPF: %s\nCurso: %s\nMae: %s\nPai: %s\nAno ingresso: %d\nCA: %.2f\n",
                       a.matricula, a.nome, a.cpf, a.curso, a.mae, a.pai, a.anoIngresso, a.ca);
            }
        }

        free(buffer);

    } else { // registros variáveis
        while (1) {
            int tam;
            if (fread(&tam, sizeof(int), 1, f) != 1) break; // chega ao fim do arquivo

            char *buffer = malloc(tam);
            if (fread(buffer, tam, 1, f) != 1) { free(buffer); break; }

            Aluno a;
            deserializarAlunoVariavel(buffer, &a);
            free(buffer);

            if (!a.removido) {
                printf("----------------------------\n");
                printf("Matricula: %d\nNome: %s\nCPF: %s\nCurso: %s\nMae: %s\nPai: %s\nAno ingresso: %d\nCA: %.2f\n",
                       a.matricula, a.nome, a.cpf, a.curso, a.mae, a.pai, a.anoIngresso, a.ca);
            }
        }
    }

    fclose(f);
}

// Adiciona registro
void adicionarRegistroArquivo(const Aluno *a, int tipo, int tamanhoBloco, int espalhado) {
    FILE *f = fopen("alunos.dat", "r+b");
    if (!f) { f = fopen("alunos.dat", "w+b"); if(!f) return; fwrite(&tipo, sizeof(int),1,f); }

    fseek(f, 0, SEEK_END);
    if (tipo == 1) {
        char buffer[1024]; serializarAlunoFixo(a, buffer);
        fwrite(buffer, tamanhoRegistroFixo(), 1, f);
    } else {
        char buffer[1024]; serializarAlunoVariavel(a, buffer);
        int tam = tamanhoRegistroVariavel(a);
        fwrite(&tam, sizeof(int),1,f);
        fwrite(buffer, tam,1,f);
    }
    fclose(f);
    printf("Registro adicionado com sucesso!\n");
}

// Marca como removido
void excluirRegistroArquivo(int matricula, int tipo, int tamanhoBloco) {
    FILE *f = fopen("alunos.dat", "r+b");
    if (!f) {
        printf("Arquivo inexistente.\n");
        return;
    }

    fseek(f, sizeof(int), SEEK_SET); // pular flag de tipo

    int encontrado = 0;

    if (tipo == 1) { // registro fixo
        int tam = tamanhoRegistroFixo();
        char buffer[1024];
        long pos;
        while (fread(buffer, tam, 1, f) == 1) {
            Aluno a;
            deserializarAlunoFixo(buffer, &a);

            if (a.matricula == matricula && a.removido == 0) {
                a.removido = 1;
                pos = ftell(f) - tam;
                fseek(f, pos, SEEK_SET);
                serializarAlunoFixo(&a, buffer);
                fwrite(buffer, tam, 1, f);
                fflush(f);
                encontrado = 1;
                break;
            }
        }
    } else { // registro variável
        while (!feof(f)) {
            int tam;
            if (fread(&tam, sizeof(int), 1, f) != 1) break;
            long pos = ftell(f);
            char *buffer = malloc(tam);
            if (fread(buffer, tam, 1, f) != 1) { free(buffer); break; }

            Aluno a;
            deserializarAlunoVariavel(buffer, &a);

            if (a.matricula == matricula && a.removido == 0) {
                a.removido = 1;
                int novoTam = tamanhoRegistroVariavel(&a);
                char *bufNovo = malloc(novoTam);
                serializarAlunoVariavel(&a, bufNovo);

                fseek(f, pos - sizeof(int), SEEK_SET);
                fwrite(&novoTam, sizeof(int), 1, f);
                fwrite(bufNovo, novoTam, 1, f);
                fflush(f);

                free(bufNovo);
                free(buffer);
                encontrado = 1;
                break;
            }
            free(buffer);
        }
    }

    if (encontrado)
        printf("Registro excluido com sucesso!\n");
    else
        printf("Matricula nao encontrada ou ja removida.\n");

    fclose(f);
}

// Edita registro existente
void editarRegistroArquivo(int matricula, int tipo, int tamanhoBloco) {
    FILE *f = fopen("alunos.dat", "r+b");
    if(!f){printf("Arquivo inexistente.\n");return;}
    fseek(f,sizeof(int),SEEK_SET);

    if(tipo==1){
        int tam=tamanhoRegistroFixo(); char buffer[1024]; long pos;
        while(fread(buffer,tam,1,f)==1){
            Aluno a; deserializarAlunoFixo(buffer,&a);
            if(a.matricula==matricula){
                printf("Editando aluno %d:\n",matricula);
                printf("Novo nome: "); scanf(" %[^\n]",a.nome);
                printf("Novo CPF: "); scanf(" %[^\n]",a.cpf);
                printf("Novo curso: "); scanf(" %[^\n]",a.curso);
                printf("Nova mae: "); scanf(" %[^\n]",a.mae);
                printf("Novo pai: "); scanf(" %[^\n]",a.pai);
                printf("Novo ano ingresso: "); scanf("%d",&a.anoIngresso);
                printf("Novo CA: "); scanf("%f",&a.ca);
                pos=ftell(f)-tam; fseek(f,pos,SEEK_SET); serializarAlunoFixo(&a,buffer); fwrite(buffer,tam,1,f);
                printf("Registro editado com sucesso!\n"); fclose(f); return;
            }
        }
    } else {
        while(1){
            int tam; if(fread(&tam,sizeof(int),1,f)!=1) break;
            long pos = ftell(f); char *buffer = malloc(tam); fread(buffer,tam,1,f);
            Aluno a; deserializarAlunoVariavel(buffer,&a);
            if(a.matricula==matricula){
                printf("Editando aluno %d:\n",matricula);
                printf("Novo nome: "); scanf(" %[^\n]",a.nome);
                printf("Novo CPF: "); scanf(" %[^\n]",a.cpf);
                printf("Novo curso: "); scanf(" %[^\n]",a.curso);
                printf("Nova mae: "); scanf(" %[^\n]",a.mae);
                printf("Novo pai: "); scanf(" %[^\n]",a.pai);
                printf("Novo ano ingresso: "); scanf("%d",&a.anoIngresso);
                printf("Novo CA: "); scanf("%f",&a.ca);
                int novoTam=tamanhoRegistroVariavel(&a);
                char *bufNovo=malloc(novoTam); serializarAlunoVariavel(&a,bufNovo);
                fseek(f,pos-sizeof(int),SEEK_SET); fwrite(&novoTam,sizeof(int),1,f); fwrite(bufNovo,novoTam,1,f);
                free(bufNovo); free(buffer); printf("Registro editado com sucesso!\n"); fclose(f); return;
            }
            free(buffer);
        }
    }
    printf("Matricula nao encontrada.\n");
    fclose(f);
}
void adicionarRegistroEspacoLivre(const Aluno *a, int tipo) {
    FILE *f = fopen("alunos.dat", "r+b");
    if (!f) return;

    fseek(f, sizeof(int), SEEK_SET); // pula flag de tipo
    int tamRegistro = tamanhoRegistroVariavel(a);
    char *bufferNovo = malloc(tamRegistro);
    serializarAlunoVariavel(a, bufferNovo);

    while (1) {
        long posInicio = ftell(f);
        int tamExistente;
        if (fread(&tamExistente, sizeof(int), 1, f) != 1) break;

        char *bufferExistente = malloc(tamExistente);
        fread(bufferExistente, tamExistente, 1, f);

        Aluno temp;
        deserializarAlunoVariavel(bufferExistente, &temp);

        if (temp.removido && tamExistente >= tamRegistro) {
            fseek(f, posInicio, SEEK_SET);
            fwrite(&tamRegistro, sizeof(int), 1, f);
            fwrite(bufferNovo, tamRegistro, 1, f);
            free(bufferNovo);
            free(bufferExistente);
            fclose(f);
            printf("Registro inserido em espaco livre!\n");
            return;
        }

        free(bufferExistente);
    }

    // Nenhum espaço livre -> insere no final
    fseek(f, 0, SEEK_END);
    fwrite(&tamRegistro, sizeof(int), 1, f);
    fwrite(bufferNovo, tamRegistro, 1, f);

    free(bufferNovo);
    fclose(f);
    printf("Registro adicionado no final!\n");
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "aluno.h"
#include "bloco.h"

// Reorganiza o arquivo "alunos.dat" compactando registros ativos
void reorganizarArquivo(int tamanhoBloco, int tipo) {
    FILE *f = fopen("alunos.dat", "rb");
    if (!f) { printf("Arquivo inexistente.\n"); return; }

    int flagTipo;
    if (fread(&flagTipo, sizeof(int), 1, f) != 1) { fclose(f); return; }

    // Cria novo arquivo temporário
    FILE *novo = fopen("alunos_temp.dat", "wb");
    if (!novo) { fclose(f); printf("Erro ao criar arquivo temporario.\n"); return; }

    // Salva o tipo de registro
    fwrite(&tipo, sizeof(int), 1, novo);

    Bloco *b = criarBloco(1, tamanhoBloco);
    int totalBlocos = 1;

    if (tipo == 1) {
        int tam = tamanhoRegistroFixo();
        char *buffer = malloc(tam);

        while (fread(buffer, tam, 1, f) == 1) {
            Aluno a;
            deserializarAlunoFixo(buffer, &a);
            if (a.removido) continue;

            serializarAlunoFixo(&a, buffer); // garante que removido=0
            int novoBloco = adicionarRegistro(b, buffer, tam, novo, 0, totalBlocos);
            if (novoBloco) totalBlocos++;
        }

        gravarBloco(novo, b);
        free(buffer);
    } else {
        while (!feof(f)) {
            int tam = 0;
            long pos = ftell(f);
            if (fread(&tam, sizeof(int), 1, f) != 1) break;

            if (tam <= 0) { fseek(f, pos + sizeof(int), SEEK_SET); continue; }

            char *buffer = malloc(tam);
            if (fread(buffer, tam, 1, f) != 1) { free(buffer); break; }

            Aluno a;
            deserializarAlunoVariavel(buffer, &a);
            free(buffer);

            if (a.removido) continue;

            int novoTam = tamanhoRegistroVariavel(&a);
            char *registroCompleto = malloc(sizeof(int) + novoTam);
            memcpy(registroCompleto, &novoTam, sizeof(int));
            serializarAlunoVariavel(&a, registroCompleto + sizeof(int));

            int novoBloco = adicionarRegistro(b, registroCompleto, sizeof(int) + novoTam, novo, 0, totalBlocos);
            if (novoBloco) totalBlocos++;

            free(registroCompleto);
        }

        gravarBloco(novo, b);
    }

    liberarBloco(b);
    fclose(f);
    fclose(novo);

    remove("alunos.dat");
    rename("alunos_temp.dat", "alunos.dat");

    printf("Arquivo reorganizado com sucesso!\n");
}
