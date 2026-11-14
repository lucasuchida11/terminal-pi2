/*
 * Projeto Integrador II - Ciberseguranca e Criptografia (PUC-Campinas)
 * Terminal simplificado em C
 * Lucas Uchida e Yuri Lopes
 *
 * Para compilar: gcc terminal.c -o terminal
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM 1000

/* Nome de quem esta logado. Vazio ("") quer dizer que ninguem esta logado. */
char usuario_logado[50] = "";

/* Lista dos comandos, usada pelo comando ajuda */
char *comandos[] = {"ajuda", "sair", "ec", "criar", "cat", "cesar",
                    "registrar", "login", "logout", "quem-sou"};
char *descricoes[] = {"mostra os comandos", "fecha o terminal", "repete um texto",
                      "cria um arquivo com um texto", "mostra o conteudo de um arquivo",
                      "cifra ou decifra com a cifra de Cesar", "cria uma conta",
                      "entra em uma conta", "sai da conta", "mostra quem esta logado"};
char *usos[] = {"ajuda [comando]", "sair", "ec <texto>", "criar <arquivo> <texto>",
                "cat <arquivo>",
                "cesar cifrar|decifrar -t <texto> <chave>  ou  -f <arquivo> <chave>",
                "registrar <usuario>", "login <usuario>", "logout", "quem-sou"};
int total_comandos = 10;

/* ------------------------------------------------------------ */
/* Funcoes de apoio                                              */
/* ------------------------------------------------------------ */

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

/* ------------------------------------------------------------ */
/* Comandos basicos                                              */
/* ------------------------------------------------------------ */

void cmd_ajuda(char linha[])
{
    char nome[50] = "";
    int i;
    sscanf(linha, "%*s %49s", nome);   /* %*s pula a palavra "ajuda" */

    if (strcmp(nome, "") == 0) {
        printf("Comandos disponiveis:\n");
        for (i = 0; i < total_comandos; i++)
            printf("  %-10s %s\n", comandos[i], descricoes[i]);
        return;
    }
    for (i = 0; i < total_comandos; i++) {
        if (strcmp(nome, comandos[i]) == 0) {
            printf("%s: %s\nUso: %s\n", comandos[i], descricoes[i], usos[i]);
            return;
        }
    }
    printf("comando desconhecido: %s\n", nome);
}

void cmd_ec(char linha[])
{
    char *texto = depois_das_palavras(linha, 1);
    if (strcmp(texto, "") == 0) {
        printf("erro: falta o texto. Uso: ec <texto>\n");
        return;
    }
    tirar_aspas(texto);
    printf("%s\n", texto);
}

void cmd_criar(char linha[])
{
    char nome[100] = "";
    char *texto;
    FILE *arquivo;

    sscanf(linha, "%*s %99s", nome);
    texto = depois_das_palavras(linha, 2);

    if (strcmp(nome, "") == 0 || strchr(nome, '/') != NULL || strchr(nome, '\\') != NULL) {
        printf("erro: nome de arquivo vazio ou invalido (nao use barras)\n");
        return;
    }
    if (strcmp(texto, "") == 0) {
        printf("erro: falta o texto. Uso: criar <arquivo> <texto>\n");
        return;
    }
    tirar_aspas(texto);

    arquivo = fopen(nome, "w");   /* "w" cria o arquivo ou apaga o que tinha */
    if (arquivo == NULL) {
        printf("erro: nao foi possivel criar o arquivo\n");
        return;
    }
    fprintf(arquivo, "%s\n", texto);
    fclose(arquivo);
    printf("arquivo %s criado\n", nome);
}

void cmd_cat(char linha[])
{
    char nome[100] = "";
    FILE *arquivo;
    int c;

    sscanf(linha, "%*s %99s", nome);
    if (strcmp(nome, "") == 0) {
        printf("erro: falta o nome do arquivo. Uso: cat <arquivo>\n");
        return;
    }
    arquivo = fopen(nome, "r");
    if (arquivo == NULL) {
        printf("arquivo nao encontrado\n");
        return;
    }
    while ((c = fgetc(arquivo)) != EOF)   /* le letra por letra ate o fim */
        putchar(c);
    fclose(arquivo);
}

/* ------------------------------------------------------------ */
/* Cifra de Cesar                                                */
/* ------------------------------------------------------------ */

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

/* ------------------------------------------------------------ */
/* Usuarios e sessao                                             */
/* ------------------------------------------------------------ */

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

/* ------------------------------------------------------------ */
/* Programa principal                                            */
/* ------------------------------------------------------------ */

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
