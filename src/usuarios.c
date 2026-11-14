/* Usuarios e sessao: registrar, login, logout e quem-sou */
#include <stdio.h>
#include <string.h>
#include "parser.h"
#include "usuarios.h"

char usuario_logado[50] = "";

/* Transforma a senha em um numero, para nao salvar a senha de verdade.
   Cada letra muda o resultado: h = h * 33 + letra */
unsigned long transformar_senha(char senha[])
{
    unsigned long h = 5381;
    int i;
    for (i = 0; senha[i] != '\0'; i++)
        h = h * 33 + senha[i];
    return h;
}

/* Procura o usuario em usuarios.txt. Se achar, devolve 1 e guarda
   a senha transformada em senha_salva. */
int buscar_usuario(char nome[], unsigned long *senha_salva)
{
    FILE *arquivo = fopen("usuarios.txt", "r");
    char nome_lido[50];
    unsigned long senha_lida;

    if (arquivo == NULL)
        return 0;   /* ainda nao tem ninguem cadastrado */
    while (fscanf(arquivo, "%49s %lu", nome_lido, &senha_lida) == 2) {
        if (strcmp(nome_lido, nome) == 0) {
            *senha_salva = senha_lida;
            fclose(arquivo);
            return 1;
        }
    }
    fclose(arquivo);
    return 0;
}

void ler_senha(char mensagem[], char senha[])
{
    printf("%s", mensagem);
    fgets(senha, 100, stdin);
    tirar_enter(senha);
}

void cmd_registrar(char linha[])
{
    char nome[50] = "", senha[100], senha2[100];
    unsigned long lixo;
    FILE *arquivo;

    sscanf(linha, "%*s %49s", nome);
    if (strcmp(nome, "") == 0) {
        printf("erro: nome invalido. Uso: registrar <usuario>\n");
        return;
    }
    if (buscar_usuario(nome, &lixo)) {
        printf("erro: o usuario %s ja existe\n", nome);
        return;
    }
    ler_senha("senha: ", senha);
    ler_senha("repita a senha: ", senha2);
    if (strcmp(senha, senha2) != 0) {
        printf("erro: as senhas sao diferentes\n");
        return;
    }
    if (strcmp(senha, "") == 0) {
        printf("erro: a senha nao pode ser vazia\n");
        return;
    }
    arquivo = fopen("usuarios.txt", "a");   /* "a" escreve no final */
    fprintf(arquivo, "%s %lu\n", nome, transformar_senha(senha));
    fclose(arquivo);
    printf("usuario %s cadastrado\n", nome);
}

void cmd_login(char linha[])
{
    char nome[50] = "", senha[100];
    unsigned long senha_salva;

    sscanf(linha, "%*s %49s", nome);
    if (strcmp(usuario_logado, "") != 0) {
        printf("erro: %s ja esta logado, use logout antes\n", usuario_logado);
        return;
    }
    if (!buscar_usuario(nome, &senha_salva)) {
        printf("erro: usuario nao existe\n");
        return;
    }
    ler_senha("senha: ", senha);
    if (transformar_senha(senha) != senha_salva) {
        printf("erro: senha incorreta\n");
        return;
    }
    strcpy(usuario_logado, nome);
    printf("bem-vindo, %s!\n", nome);
}

void cmd_logout()
{
    if (strcmp(usuario_logado, "") == 0) {
        printf("nao ha usuario logado\n");
        return;
    }
    printf("sessao de %s encerrada\n", usuario_logado);
    strcpy(usuario_logado, "");
}

void cmd_quem_sou()
{
    if (strcmp(usuario_logado, "") == 0)
        printf("nenhuma sessao ativa\n");
    else
        printf("%s\n", usuario_logado);
}
