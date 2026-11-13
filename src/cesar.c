#include "cesar.h"

char cesar_char(char c, int desloc)
{
    if (c >= 'A' && c <= 'Z')
        return (char)('A' + (c - 'A' + desloc) % 26);
    if (c >= 'a' && c <= 'z')
        return (char)('a' + (c - 'a' + desloc) % 26);
    return c; /* nao e letra: fica igual */
}

void cesar_texto(char *texto, int desloc)
{
    int i;
    for (i = 0; texto[i] != '\0'; i++)
        texto[i] = cesar_char(texto[i], desloc);
}

void cesar_arquivo(FILE *entrada, FILE *saida, int desloc)
{
    int c;
    while ((c = fgetc(entrada)) != EOF)
        fputc(cesar_char((char)c, desloc), saida);
}
