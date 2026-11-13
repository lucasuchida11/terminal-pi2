#ifndef CESAR_H
#define CESAR_H

#include <stdio.h>

/*
 * Cifra de Cesar: cada letra anda 'desloc' posicoes no alfabeto, de forma
 * circular (depois do Z vem o A). Matematicamente, numerando A=0 ... Z=25:
 *
 *     cifrar:   c = (p + k) mod 26
 *     decifrar: p = (c - k) mod 26 = (c + 26 - k) mod 26
 *
 * Maiusculas continuam maiusculas, minusculas continuam minusculas, e
 * qualquer outro caractere (espaco, numero, pontuacao) nao muda.
 */

/* Desloca um unico caractere. */
char cesar_char(char c, int desloc);

/* Aplica a cifra no texto inteiro, alterando a propria string. */
void cesar_texto(char *texto, int desloc);

/* Le 'entrada' caractere a caractere e grava o resultado em 'saida'. */
void cesar_arquivo(FILE *entrada, FILE *saida, int desloc);

#endif
