/* Cifra de Cesar */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "cesar.h"

/* Anda k posicoes no alfabeto. O % 26 faz o Z voltar para o A. */
char cesar_letra(char c, int k)
{
    if (c >= 'A' && c <= 'Z')
        return 'A' + (c - 'A' + k) % 26;
    if (c >= 'a' && c <= 'z')
        return 'a' + (c - 'a' + k) % 26;
    return c;   /* espaco, numero e pontuacao nao mudam */
}

/* Formato: cesar cifrar|decifrar -t|-f <texto ou arquivo> <chave> */
void cmd_cesar(char linha[])
{
    char operacao[20] = "", modo[5] = "", resto[TAM];
    char *chave, *espaco;
    int k, i, c;

    sscanf(linha, "%*s %19s %4s", operacao, modo);
    strcpy(resto, depois_das_palavras(linha, 3));   /* sobra "<texto> <chave>" */

    /* a chave e a ultima palavra da linha */
    espaco = strrchr(resto, ' ');
    if (strcmp(operacao, "") == 0 || strcmp(modo, "") == 0 || espaco == NULL) {
        printf("erro: faltam argumentos. Digite: ajuda cesar\n");
        return;
    }
    *espaco = '\0';          /* corta o resto em dois: texto e chave */
    chave = espaco + 1;

    for (i = 0; chave[i] != '\0'; i++) {
        if (chave[i] < '0' || chave[i] > '9') {
            printf("erro: a chave precisa ser um numero de 0 a 25\n");
            return;
        }
    }
    k = atoi(chave);
    if (k > 25) {
        printf("erro: a chave precisa ser um numero de 0 a 25\n");
        return;
    }

    if (strcmp(operacao, "decifrar") == 0)
        k = (26 - k) % 26;   /* voltar k posicoes = andar 26 - k */
    else if (strcmp(operacao, "cifrar") != 0) {
        printf("erro: use cifrar ou decifrar\n");
        return;
    }

    if (strcmp(modo, "-t") == 0) {
        tirar_aspas(resto);
        for (i = 0; resto[i] != '\0'; i++)
            printf("%c", cesar_letra(resto[i], k));
        printf("\n");
    } else if (strcmp(modo, "-f") == 0) {
        char nome_saida[TAM + 20];
        FILE *entrada, *saida;

        entrada = fopen(resto, "r");
        if (entrada == NULL) {
            printf("arquivo nao encontrado\n");
            return;
        }
        if (strcmp(operacao, "cifrar") == 0)
            sprintf(nome_saida, "%s.cifrado", resto);
        else
            sprintf(nome_saida, "%s.decifrado", resto);

        saida = fopen(nome_saida, "w");
        while ((c = fgetc(entrada)) != EOF)
            fputc(cesar_letra(c, k), saida);
        fclose(entrada);
        fclose(saida);
        printf("resultado salvo em %s\n", nome_saida);
    } else {
        printf("erro: use -t para texto ou -f para arquivo\n");
    }
}
