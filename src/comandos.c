#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "comandos.h"
#include "parser.h"
#include "cesar.h"
#include "usuarios.h"

/* Declaracao antecipada: as funcoes estao definidas mais abaixo. */
static int cmd_ajuda(int argc, char *argv[]);
static int cmd_sair(int argc, char *argv[]);
static int cmd_ec(int argc, char *argv[]);
static int cmd_criar(int argc, char *argv[]);
static int cmd_cat(int argc, char *argv[]);
static int cmd_cesar(int argc, char *argv[]);
static int cmd_registrar(int argc, char *argv[]);
static int cmd_login(int argc, char *argv[]);
static int cmd_logout(int argc, char *argv[]);
static int cmd_quem_sou(int argc, char *argv[]);

/* ------------------------------------------------------------------ */
/* Tabela de comandos                                                  */
/* ------------------------------------------------------------------ */
static const comando COMANDOS[] = {
    { "ajuda", "Lista os comandos ou mostra a ajuda de um comando",
      "ajuda [comando]",
      "ajuda cesar", cmd_ajuda },
    { "sair", "Encerra a sessao e o programa",
      "sair",
      "sair", cmd_sair },
    { "ec", "Repete um texto na tela",
      "ec <texto>",
      "ec ola mundo", cmd_ec },
    { "criar", "Cria um arquivo com um texto",
      "criar <arquivo> <texto>",
      "criar msg.txt def", cmd_criar },
    { "cat", "Mostra o conteudo de um arquivo",
      "cat <arquivo>",
      "cat msg.txt", cmd_cat },
    { "cesar", "Cifra ou decifra um texto/arquivo com a cifra de Cesar",
      "cesar cifrar|decifrar -t <texto> <deslocamento>\n"
      "         cesar cifrar|decifrar -f <arquivo> <deslocamento>\n"
      "         <deslocamento> vai de 0 a 25. Com -f o resultado vai para\n"
      "         <arquivo>.cifrado ou <arquivo>.decifrado",
      "cesar cifrar -t ABC 3   (mostra DEF)", cmd_cesar },
    { "registrar", "Cria uma conta de usuario",
      "registrar <usuario>   (a senha e pedida duas vezes)",
      "registrar ana", cmd_registrar },
    { "login", "Inicia uma sessao",
      "login <usuario>   (a senha e pedida em seguida)",
      "login ana", cmd_login },
    { "logout", "Encerra a sessao atual",
      "logout",
      "logout", cmd_logout },
    { "quem-sou", "Informa o usuario da sessao",
      "quem-sou",
      "quem-sou", cmd_quem_sou },
};

#define NUM_COMANDOS ((int)(sizeof(COMANDOS) / sizeof(COMANDOS[0])))

static const comando *buscar_comando(const char *nome)
{
    int i;
    for (i = 0; i < NUM_COMANDOS; i++)
        if (strcmp(COMANDOS[i].nome, nome) == 0)
            return &COMANDOS[i];
    return NULL;
}

int executar_comando(int argc, char *argv[])
{
    const comando *c;

    if (argc == 0)
        return CONTINUAR; /* linha vazia */

    c = buscar_comando(argv[0]);
    if (c == NULL) {
        printf("comando desconhecido: %s (digite 'ajuda')\n", argv[0]);
        return CONTINUAR;
    }
    return c->executar(argc, argv);
}

/* ------------------------------------------------------------------ */
/* Nucleo do terminal                                                  */
/* ------------------------------------------------------------------ */

static int cmd_ajuda(int argc, char *argv[])
{
    int i;
    const comando *c;

    if (argc == 1) {
        printf("Comandos disponiveis:\n");
        for (i = 0; i < NUM_COMANDOS; i++)
            printf("  %-10s %s\n", COMANDOS[i].nome, COMANDOS[i].descricao);
        printf("Digite 'ajuda <comando>' para ver detalhes.\n");
        return CONTINUAR;
    }

    c = buscar_comando(argv[1]);
    if (c == NULL) {
        printf("comando desconhecido: %s\n", argv[1]);
        return CONTINUAR;
    }
    printf("%s - %s\n", c->nome, c->descricao);
    printf("Sintaxe: %s\n", c->sintaxe);
    printf("Exemplo: %s\n", c->exemplo);
    return CONTINUAR;
}

static int cmd_sair(int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    if (sessao_usuario()[0] != '\0') {
        printf("sessao de %s encerrada.\n", sessao_usuario());
        sessao_encerrar();
    }
    printf("Ate logo!\n");
    return ENCERRAR;
}

static int cmd_ec(int argc, char *argv[])
{
    char texto[MAX_LINHA];

    if (argc < 2) {
        printf("erro: falta o texto. Uso: ec <texto>\n");
        return CONTINUAR;
    }
    juntar_argumentos(texto, sizeof(texto), argv, 1, argc);
    printf("%s\n", texto);
    return CONTINUAR;
}

/* O nome do arquivo nao pode ser vazio nem conter caminhos com barras. */
static int nome_arquivo_valido(const char *nome)
{
    if (nome[0] == '\0')
        return 0;
    if (strchr(nome, '/') != NULL || strchr(nome, '\\') != NULL)
        return 0;
    return 1;
}

static int cmd_criar(int argc, char *argv[])
{
    char texto[MAX_LINHA];
    FILE *f;

    if (argc < 2 || !nome_arquivo_valido(argv[1])) {
        printf("erro: nome de arquivo vazio ou invalido (nao use barras).\n");
        return CONTINUAR;
    }
    if (argc < 3) {
        printf("erro: falta o texto. Uso: criar <arquivo> <texto>\n");
        return CONTINUAR;
    }

    juntar_argumentos(texto, sizeof(texto), argv, 2, argc);

    f = fopen(argv[1], "w"); /* "w" substitui o conteudo se ja existir */
    if (f == NULL) {
        printf("erro: falha ao escrever o arquivo %s\n", argv[1]);
        return CONTINUAR;
    }
    fprintf(f, "%s\n", texto);
    fclose(f);
    printf("arquivo %s criado.\n", argv[1]);
    return CONTINUAR;
}

static int cmd_cat(int argc, char *argv[])
{
    FILE *f;
    int c;

    if (argc < 2) {
        printf("erro: falta o nome do arquivo. Uso: cat <arquivo>\n");
        return CONTINUAR;
    }
    f = fopen(argv[1], "r");
    if (f == NULL) {
        printf("arquivo nao encontrado: %s\n", argv[1]);
        return CONTINUAR;
    }
    while ((c = fgetc(f)) != EOF)
        putchar(c);
    fclose(f);
    return CONTINUAR;
}

/* ------------------------------------------------------------------ */
/* Cifra de Cesar                                                      */
/* ------------------------------------------------------------------ */

/* Converte o texto em numero e confere se esta entre 0 e 25.
 * Retorna 1 se deu certo (valor em *desloc), 0 caso contrario. */
static int ler_deslocamento(const char *s, int *desloc)
{
    char *fim;
    long v = strtol(s, &fim, 10);

    if (s[0] == '\0' || *fim != '\0')
        return 0; /* nao e um numero inteiro */
    if (v < 0 || v > 25)
        return 0;
    *desloc = (int)v;
    return 1;
}

/*
 * Formato: cesar <cifrar|decifrar> <-t|-f> <texto ou arquivo> <deslocamento>
 *   argv[0]      = "cesar"
 *   argv[1]      = operacao
 *   argv[2]      = modo (-t ou -f)
 *   argv[3..n-2] = texto (pode ser varias palavras) ou nome do arquivo
 *   argv[n-1]    = deslocamento
 */
static int cmd_cesar(int argc, char *argv[])
{
    int cifrar, desloc;
    const char *operacao, *modo;

    if (argc < 5) {
        printf("erro: faltam argumentos. Digite 'ajuda cesar'.\n");
        return CONTINUAR;
    }
    operacao = argv[1];
    modo = argv[2];

    if (strcmp(operacao, "cifrar") == 0)
        cifrar = 1;
    else if (strcmp(operacao, "decifrar") == 0)
        cifrar = 0;
    else {
        printf("erro: operacao '%s' invalida. Use cifrar ou decifrar.\n", operacao);
        return CONTINUAR;
    }

    if (!ler_deslocamento(argv[argc - 1], &desloc)) {
        printf("erro: deslocamento '%s' invalido. Use um numero de 0 a 25.\n",
               argv[argc - 1]);
        return CONTINUAR;
    }

    /* Decifrar com k e o mesmo que cifrar com 26 - k. */
    if (!cifrar)
        desloc = (26 - desloc) % 26;

    if (strcmp(modo, "-t") == 0) {
        char texto[MAX_LINHA];
        juntar_argumentos(texto, sizeof(texto), argv, 3, argc - 1);
        cesar_texto(texto, desloc);
        printf("%s\n", texto);
    } else if (strcmp(modo, "-f") == 0) {
        char nome_saida[MAX_LINHA];
        FILE *entrada, *saida;

        if (argc != 5) {
            printf("erro: informe apenas um arquivo. Uso: cesar %s -f <arquivo> <deslocamento>\n",
                   operacao);
            return CONTINUAR;
        }
        entrada = fopen(argv[3], "r");
        if (entrada == NULL) {
            printf("arquivo nao encontrado: %s\n", argv[3]);
            return CONTINUAR;
        }
        snprintf(nome_saida, sizeof(nome_saida), "%s.%s",
                 argv[3], cifrar ? "cifrado" : "decifrado");
        saida = fopen(nome_saida, "w");
        if (saida == NULL) {
            printf("erro: falha ao escrever o arquivo %s\n", nome_saida);
            fclose(entrada);
            return CONTINUAR;
        }
        cesar_arquivo(entrada, saida, desloc);
        fclose(entrada);
        fclose(saida);
        printf("resultado gravado em %s\n", nome_saida);
    } else {
        printf("erro: modo '%s' invalido. Use -t (texto) ou -f (arquivo).\n", modo);
    }
    return CONTINUAR;
}

/* ------------------------------------------------------------------ */
/* Sessao e usuarios                                                   */
/* ------------------------------------------------------------------ */

static int cmd_registrar(int argc, char *argv[])
{
    char senha[MAX_SENHA], confirmacao[MAX_SENHA];

    if (argc < 2) {
        printf("erro: falta o nome de usuario. Uso: registrar <usuario>\n");
        return CONTINUAR;
    }
    if (!usuario_valido(argv[1])) {
        printf("erro: nome invalido. Use de 1 a %d letras, digitos, '_' ou '-'.\n",
               MAX_USUARIO);
        return CONTINUAR;
    }
    if (usuario_existe(argv[1])) {
        printf("erro: o usuario %s ja existe.\n", argv[1]);
        return CONTINUAR;
    }

    if (!ler_senha("senha: ", senha, sizeof(senha)) ||
        !ler_senha("repita a senha: ", confirmacao, sizeof(confirmacao)))
        return CONTINUAR;

    if (strcmp(senha, confirmacao) != 0) {
        printf("erro: as senhas sao diferentes.\n");
        return CONTINUAR;
    }
    if ((int)strlen(senha) < MIN_SENHA) {
        printf("erro: a senha precisa ter pelo menos %d caracteres.\n", MIN_SENHA);
        return CONTINUAR;
    }
    if (!salvar_usuario(argv[1], senha)) {
        printf("erro: falha ao gravar o arquivo de usuarios.\n");
        return CONTINUAR;
    }
    printf("usuario %s cadastrado.\n", argv[1]);
    return CONTINUAR;
}

static int cmd_login(int argc, char *argv[])
{
    char senha[MAX_SENHA];

    if (argc < 2) {
        printf("erro: falta o nome de usuario. Uso: login <usuario>\n");
        return CONTINUAR;
    }
    if (sessao_usuario()[0] != '\0') {
        printf("erro: %s ja esta logado. Use logout antes.\n", sessao_usuario());
        return CONTINUAR;
    }
    if (!usuario_existe(argv[1])) {
        printf("erro: usuario %s nao existe.\n", argv[1]);
        return CONTINUAR;
    }
    if (!ler_senha("senha: ", senha, sizeof(senha)))
        return CONTINUAR;

    if (!conferir_senha(argv[1], senha)) {
        printf("erro: senha incorreta.\n");
        return CONTINUAR;
    }
    sessao_iniciar(argv[1]);
    printf("sessao iniciada. Bem-vindo, %s!\n", argv[1]);
    return CONTINUAR;
}

static int cmd_logout(int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    if (sessao_usuario()[0] == '\0') {
        printf("nao ha usuario logado.\n");
        return CONTINUAR;
    }
    printf("sessao de %s encerrada.\n", sessao_usuario());
    sessao_encerrar();
    return CONTINUAR;
}

static int cmd_quem_sou(int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    if (sessao_usuario()[0] == '\0')
        printf("nenhuma sessao ativa.\n");
    else
        printf("%s\n", sessao_usuario());
    return CONTINUAR;
}
