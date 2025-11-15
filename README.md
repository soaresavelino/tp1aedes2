# Trabalho Prático 01 - Manipulação e Organização de Arquivos de Dados

## Disciplina
Algoritmos e Estruturas de Dados II

## Descrição
Este programa simula a persistência de registros de alunos em um arquivo binário (`.dat`), considerando:

- Armazenamento em blocos de tamanho fixo;
- Registros de tamanho fixo ou variável;
- Registros contíguos ou espalhados (no caso de registros variáveis);
- Estatísticas de ocupação dos blocos e eficiência de armazenamento.

## Arquivos

- `main.c` – Programa principal, interface de menu.
- `aluno.c` / `aluno.h` – Estrutura do registro de aluno e funções de serialização/deserialização.
- `bloco.c` / `bloco.h` – Estrutura de blocos e funções de gerenciamento.
- `gerador.c` – Funções para gerar registros de alunos fictícios.
- `salvar_original.c` – Funções para criar o arquivo de backup `alunos_original.dat`.
- `alunos.dat` – Arquivo principal gerado pelo programa.
- `alunos_original.dat` – Backup do arquivo original

## Requisitos

- GCC (MinGW no Windows ou GCC no Linux/macOS)
- Terminal ou Prompt de Comando

## Compilação

Abra o terminal na pasta do projeto e execute o comando:

```bash
gcc main.c aluno.c bloco.c gerador.c salvar_original.c -o tp1.exe
