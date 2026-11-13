#include <string.h>
#include "parser.h"

int separar_argumentos(char *linha, char *argv[], int max_args)
{
    int argc = 0;
    char *p = linha;

    while (*p != '\0' && argc < max_args) {
        /* pula espacos antes do argumento */
        while (*p == ' ' || *p == '\t')
            p++;
        if (*p == '\0')
            break;

        if (*p == '"') {
            /* argumento entre aspas: vai ate a proxima aspa */
            p++;
            argv[argc++] = p;
            while (*p != '\0' && *p != '"')
                p++;
        } else {
            /* argumento comum: vai ate o proximo espaco */
            argv[argc++] = p;
            while (*p != '\0' && *p != ' ' && *p != '\t')
                p++;
        }

        if (*p != '\0') {
            *p = '\0'; /* termina o argumento atual */
            p++;
        }
    }
    return argc;
}

void juntar_argumentos(char *destino, int tam, char *argv[], int inicio, int fim)
{
    int i;
    destino[0] = '\0';
    for (i = inicio; i < fim; i++) {
        if (i > inicio)
            strncat(destino, " ", tam - strlen(destino) - 1);
        strncat(destino, argv[i], tam - strlen(destino) - 1);
    }
}
