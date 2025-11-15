#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "aluno.c"
#include "bloco.c"
#include "gerador.c"
#include "salvar_original.c"

void exibirMapaAtual(int tamanhoBloco) {
    FILE *arquivo = fopen("alunos.dat", "rb");
    if (!arquivo) { perror("Erro ao abrir arquivo"); return; }

    int tipo;
    fread(&tipo, sizeof(int), 1, arquivo);

    Bloco *b = criarBloco(1, tamanhoBloco);
    int totalBlocos = 0;
    int ocupacao[1000];

    while (fread(b->dados, 1, tamanhoBloco, arquivo) > 0) {
        b->bytesUsados = 0;
        // contar bytes ocupados: assumir que cada byte != 0 é ocupado
        for (int i = 0; i < tamanhoBloco; i++) {
            if (b->dados[i] != 0) b->bytesUsados++;
        }
        ocupacao[totalBlocos] = b->bytesUsados;
        totalBlocos++;
        if (feof(arquivo)) break;
    }
    fclose(arquivo);

    printf("===== Mapa de Ocupacao =====\n");
    int blocosParciais = 0;
    int soma = 0;
    for (int i = 0; i < totalBlocos; i++) {
        float perc = (ocupacao[i] * 100.0) / tamanhoBloco;
        printf("Bloco %d: %d bytes (%.1f%% cheio)\n", i + 1, ocupacao[i], perc);
        soma += ocupacao[i];
        if (ocupacao[i] < tamanhoBloco) blocosParciais++;
    }
    printf("\nTotal de blocos: %d\n", totalBlocos);
    printf("Blocos parcialmente usados: %d\n", blocosParciais);
    printf("Ocupacao media: %.1f%%\n", (soma * 100.0) / (totalBlocos * tamanhoBloco));
    printf("Eficiencia total: %.1f%%\n", (soma * 100.0) / (totalBlocos * tamanhoBloco));

    liberarBloco(b);
}

int main() {
    int n, tamanhoBloco, tipo, espalhado = 0;

    printf("Quantidade de registros iniciais: ");
    scanf("%d", &n);
    printf("Tamanho do bloco (bytes): ");
    scanf("%d", &tamanhoBloco);

    printf("\nModo de armazenamento:\n1 - Registros de tamanho fixo\n2 - Registros de tamanho variavel\n> ");
    scanf("%d", &tipo);

    if (tipo == 2) {
        printf("\n1 - Contiguos\n2 - Espalhados\n> ");
        int esc;
        scanf("%d", &esc);
        espalhado = (esc == 2);
    }

    Aluno *alunos = gerarListaAlunos(n);
    salvarOriginal(alunos, n);
    printf("Arquivo alunos_original.dat gerado com sucesso.\n\n");

    FILE *arquivo = fopen("alunos.dat", "wb");
    if (!arquivo) { perror("Erro ao criar arquivo"); return 1; }
    fwrite(&tipo, sizeof(int), 1, arquivo);

    Bloco *b = criarBloco(1, tamanhoBloco);
    int totalBlocos = 1;

    for (int i = 0; i < n; i++) {
        char buffer[1500];
        int tam;
        if (tipo == 1) {
            tam = tamanhoRegistroFixo();
            serializarAlunoFixo(&alunos[i], buffer);
        } else {
            alunos[i].removido = 0;
            serializarAlunoVariavel(&alunos[i], buffer);
            tam = tamanhoRegistroVariavel(&alunos[i]);
            char registroCompleto[1600];
            memcpy(registroCompleto, &tam, sizeof(int));
            memcpy(registroCompleto + sizeof(int), buffer, tam);
            memcpy(buffer, registroCompleto, tam + sizeof(int));
            tam += sizeof(int);
        }
        int novo = adicionarRegistro(b, buffer, tam, arquivo, espalhado, totalBlocos);
        if (novo) totalBlocos++;
    }

    gravarBloco(arquivo, b);
    fclose(arquivo);

    exibirMapaAtual(tamanhoBloco);

    free(alunos);
    liberarBloco(b);

    int opcao;
    do {
        printf("\n===== MENU =====\n");
        printf("1 - Inserir novo aluno\n2 - Editar aluno existente\n3 - Excluir aluno\n4 - Visualizar todos os registros\n5 - Reorganizar arquivo (compactar)\n0 - Sair\n> ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1: {
                Aluno novo;
                printf("Matricula: "); scanf("%d", &novo.matricula);
                printf("Nome: "); scanf(" %[^\n]", novo.nome);
                printf("CPF: "); scanf(" %[^\n]", novo.cpf);
                printf("Curso: "); scanf(" %[^\n]", novo.curso);
                printf("Mae: "); scanf(" %[^\n]", novo.mae);
                printf("Pai: "); scanf(" %[^\n]", novo.pai);
                printf("Ano ingresso: "); scanf("%d", &novo.anoIngresso);
                printf("CA: "); scanf("%f", &novo.ca);
                novo.removido = 0;
                adicionarRegistroArquivo(&novo, tipo, tamanhoBloco, espalhado);
                break;
            }
            case 2: {
                int matricula;
                printf("Matricula do aluno a editar: "); scanf("%d", &matricula);
                editarRegistroArquivo(matricula, tipo, tamanhoBloco);
                break;
            }
            case 3: {
                int matricula;
                printf("Matricula do aluno a excluir: "); scanf("%d", &matricula);
                excluirRegistroArquivo(matricula, tipo, tamanhoBloco);
                break;
            }
            case 4:
                listarRegistros();
                break;
            case 5:
                reorganizarArquivo(tamanhoBloco, tipo);
                printf("Arquivo reorganizado com sucesso!\n");
                exibirMapaAtual(tamanhoBloco); // mapa atualizado após reorganização
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida!\n");
        }

    } while(opcao != 0);

    return 0;
}
