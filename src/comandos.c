/* Comandos basicos: ajuda, ec, criar e cat */
#include <stdio.h>
#include <string.h>
#include "parser.h"
#include "comandos.h"

/* Lista dos comandos, usada pelo comando ajuda */
char *comandos[] = {"ajuda", "sair", "ec", "criar", "cat", "cesar",
                    "registrar", "login", "logout", "quem-sou"};
char *descricoes[] = {"mostra os comandos", "fecha o terminal", "repete um texto",
                      "cria um arquivo com um texto", "mostra o conteudo de um arquivo",
                      "cifra ou decifra com a cifra de Cesar", "cria uma conta",
                      "entra em uma conta", "sai da conta", "mostra quem esta logado"};
char *usos[] = {"ajuda [comando]", "sair", "ec <texto>", "criar <arquivo> <texto>",
                "cat <arquivo>",
                "cesar cifrar|decifrar -t <texto> <chave>  ou  -f <arquivo> <chave>",
                "registrar <usuario>", "login <usuario>", "logout", "quem-sou"};
int total_comandos = 10;

void cmd_ajuda(char linha[])
{
    char nome[50] = "";
    int i;
    sscanf(linha, "%*s %49s", nome);   /* %*s pula a palavra "ajuda" */

    if (strcmp(nome, "") == 0) {
        printf("Comandos disponiveis:\n");
        for (i = 0; i < total_comandos; i++)
            printf("  %-10s %s\n", comandos[i], descricoes[i]);
        return;
    }
    for (i = 0; i < total_comandos; i++) {
        if (strcmp(nome, comandos[i]) == 0) {
            printf("%s: %s\nUso: %s\n", comandos[i], descricoes[i], usos[i]);
            return;
        }
    }
    printf("comando desconhecido: %s\n", nome);
}

void cmd_ec(char linha[])
{
    char *texto = depois_das_palavras(linha, 1);
    if (strcmp(texto, "") == 0) {
        printf("erro: falta o texto. Uso: ec <texto>\n");
        return;
    }
    tirar_aspas(texto);
    printf("%s\n", texto);
}

void cmd_criar(char linha[])
{
    char nome[100] = "";
    char *texto;
    FILE *arquivo;

    sscanf(linha, "%*s %99s", nome);
    texto = depois_das_palavras(linha, 2);

    if (strcmp(nome, "") == 0 || strchr(nome, '/') != NULL || strchr(nome, '\\') != NULL) {
        printf("erro: nome de arquivo vazio ou invalido (nao use barras)\n");
        return;
    }
    if (strcmp(texto, "") == 0) {
        printf("erro: falta o texto. Uso: criar <arquivo> <texto>\n");
        return;
    }
    tirar_aspas(texto);

    arquivo = fopen(nome, "w");   /* "w" cria o arquivo ou apaga o que tinha */
    if (arquivo == NULL) {
        printf("erro: nao foi possivel criar o arquivo\n");
        return;
    }
    fprintf(arquivo, "%s\n", texto);
    fclose(arquivo);
    printf("arquivo %s criado\n", nome);
}

void cmd_cat(char linha[])
{
    char nome[100] = "";
    FILE *arquivo;
    int c;

    sscanf(linha, "%*s %99s", nome);
    if (strcmp(nome, "") == 0) {
        printf("erro: falta o nome do arquivo. Uso: cat <arquivo>\n");
        return;
    }
    arquivo = fopen(nome, "r");
    if (arquivo == NULL) {
        printf("arquivo nao encontrado\n");
        return;
    }
    while ((c = fgetc(arquivo)) != EOF)   /* le letra por letra ate o fim */
        putchar(c);
    fclose(arquivo);
}
