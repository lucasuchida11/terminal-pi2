#ifndef PARSER_H
#define PARSER_H

#define MAX_LINHA 1024
#define MAX_ARGS  64

/*
 * Separa a linha digitada em argumentos (argc/argv), como o shell faz.
 * Espacos separam argumentos; um trecho entre aspas duplas vira um
 * argumento so, mesmo contendo espacos. Ex.: ec "ola mundo" -> 2 args.
 *
 * A funcao modifica a propria linha (troca separadores por '\0') e
 * guarda em argv ponteiros para dentro dela. Retorna argc.
 */
int separar_argumentos(char *linha, char *argv[], int max_args);

/*
 * Junta argv[inicio..fim-1] em destino, separados por um espaco.
 * Usado quando o texto vem sem aspas: ec ola mundo -> "ola mundo".
 */
void juntar_argumentos(char *destino, int tam, char *argv[], int inicio, int fim);

#endif
