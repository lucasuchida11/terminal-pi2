/* Funcoes para tratar a linha digitada */
#include <string.h>
#include "parser.h"

/* Tira o Enter (\n) do final do texto lido com fgets */
void tirar_enter(char texto[])
{
    int n = strlen(texto);
    while (n > 0 && (texto[n - 1] == '\n' || texto[n - 1] == '\r')) {
        texto[n - 1] = '\0';
        n--;
    }
}

/* Se o texto estiver entre aspas, tira as aspas */
void tirar_aspas(char texto[])
{
    int i, n = strlen(texto);
    if (n >= 2 && texto[0] == '"' && texto[n - 1] == '"') {
        for (i = 0; i < n - 2; i++)
            texto[i] = texto[i + 1];
        texto[n - 2] = '\0';
    }
}

/* Devolve o que vem depois das primeiras n palavras da linha.
   Ex.: depois_das_palavras("criar a.txt ola mundo", 2) -> "ola mundo" */
char *depois_das_palavras(char linha[], int n)
{
    int i = 0;
    while (n > 0) {
        while (linha[i] == ' ')
            i++;                                   /* pula os espacos */
        while (linha[i] != ' ' && linha[i] != '\0')
            i++;                                   /* pula a palavra */
        n--;
    }
    while (linha[i] == ' ')
        i++;
    return &linha[i];
}
