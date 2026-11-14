/*
 * Projeto Integrador II - Ciberseguranca e Criptografia (PUC-Campinas)
 * Terminal simplificado em C
 * Lucas Uchida e Yuri Lopes
 */
#include <stdio.h>
#include <string.h>
#include "parser.h"
#include "comandos.h"
#include "cesar.h"
#include "usuarios.h"

int main()
{
    char linha[TAM], comando[50];

    printf("Terminal PI2 - digite ajuda para ver os comandos\n");

    while (1) {
        printf("%s> ", usuario_logado);          /* ex.: "ana> " */
        if (fgets(linha, TAM, stdin) == NULL)
            break;                               /* acabou a entrada */
        tirar_enter(linha);

        strcpy(comando, "");
        sscanf(linha, "%49s", comando);          /* primeira palavra */

        if (strcmp(comando, "") == 0)
            continue;                            /* linha vazia */
        else if (strcmp(comando, "sair") == 0) {
            if (strcmp(usuario_logado, "") != 0)
                printf("sessao de %s encerrada\n", usuario_logado);
            printf("ate logo!\n");
            break;
        }
        else if (strcmp(comando, "ajuda") == 0)     cmd_ajuda(linha);
        else if (strcmp(comando, "ec") == 0)        cmd_ec(linha);
        else if (strcmp(comando, "criar") == 0)     cmd_criar(linha);
        else if (strcmp(comando, "cat") == 0)       cmd_cat(linha);
        else if (strcmp(comando, "cesar") == 0)     cmd_cesar(linha);
        else if (strcmp(comando, "registrar") == 0) cmd_registrar(linha);
        else if (strcmp(comando, "login") == 0)     cmd_login(linha);
        else if (strcmp(comando, "logout") == 0)    cmd_logout();
        else if (strcmp(comando, "quem-sou") == 0)  cmd_quem_sou();
        else
            printf("comando desconhecido: %s (digite ajuda)\n", comando);
    }
    return 0;
}
