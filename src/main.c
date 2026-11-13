/*
 * Projeto Integrador II - Ciberseguranca e Criptografia (PUC-Campinas)
 * Terminal simplificado em C.
 *
 * Ciclo do interpretador de comandos:
 *   1. mostra o prompt
 *   2. le a linha digitada
 *   3. separa a linha em argumentos
 *   4. executa o comando correspondente
 *   5. repete ate o usuario digitar "sair" (ou fim da entrada)
 */
#include <stdio.h>
#include <string.h>
#include "parser.h"
#include "comandos.h"
#include "usuarios.h"

/* Remove o '\n' (e o '\r' do Windows) do fim da linha lida por fgets. */
static void remover_quebra_linha(char *s)
{
    s[strcspn(s, "\r\n")] = '\0';
}

int main(void)
{
    char linha[MAX_LINHA];
    char *argv[MAX_ARGS];
    int argc;
    int estado = CONTINUAR;

    printf("Terminal PI2 - digite 'ajuda' para ver os comandos.\n");

    while (estado == CONTINUAR) {
        /* O prompt mostra quem esta logado, ex.: "ana> " */
        printf("%s> ", sessao_usuario());
        fflush(stdout);

        if (fgets(linha, sizeof(linha), stdin) == NULL) {
            printf("\n");
            break; /* fim da entrada (Ctrl+D / Ctrl+Z) */
        }
        remover_quebra_linha(linha);

        argc = separar_argumentos(linha, argv, MAX_ARGS);
        estado = executar_comando(argc, argv);
    }
    return 0;
}
