Trabalho Prático 01 - Manipulação e Organização de Arquivos de Dados
Disciplina

Algoritmos e Estruturas de Dados II

Descrição

Este programa simula a persistência de registros de alunos em um arquivo binário (.dat), considerando:

Armazenamento em blocos de tamanho fixo;

Registros de tamanho fixo ou variável;

Registros contíguos ou espalhados (no caso de registros variáveis);

Estatísticas de ocupação dos blocos e eficiência de armazenamento.

Arquivos

main.c – Programa principal, interface de menu.

aluno.c / aluno.h – Estrutura do registro de aluno e funções de serialização/deserialização.

bloco.c / bloco.h – Estrutura de blocos e funções de gerenciamento.

gerador.c – Funções para gerar registros de alunos fictícios.

salvar_original.c – Funções para criar o arquivo de backup alunos_original.dat.

alunos.dat – Arquivo principal gerado pelo programa.

alunos_original.dat – Backup do arquivo original.

Requisitos

GCC (MinGW no Windows ou GCC no Linux/macOS)

Terminal ou Prompt de Comando

Compilação

Abra o terminal na pasta do projeto e execute os comandos:

gcc main.c -o tp1
./tp1.exe

Continuação – Trabalho Prático 02 (TP2)

O Trabalho Prático 02 é uma continuação do TP1 e tem como objetivo aprimorar o gerenciamento dos registros armazenados em arquivo binário. Neste trabalho, foram implementadas operações de inserção, edição e exclusão lógica de registros, permitindo o reaproveitamento do espaço liberado dentro dos blocos.

A exclusão dos registros é feita de forma lógica, ou seja, os dados não são removidos fisicamente do arquivo, mas marcados como excluídos. Esse espaço pode ser reutilizado em futuras inserções, reduzindo o desperdício de armazenamento.

Também foi implementada a funcionalidade de reorganização (compactação) do arquivo, que realoca apenas os registros válidos de forma contínua, diminuindo a fragmentação interna e melhorando a ocupação dos blocos.

Por fim, o sistema apresenta estatísticas, permitindo comparar a situação do arquivo antes e depois da reorganização e analisar os ganhos obtidos com a compactação.
