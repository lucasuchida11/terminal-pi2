#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "usuarios.h"

#ifdef _WIN32
#include <conio.h>  /* _getch: le uma tecla sem mostrar na tela */
#include <io.h>     /* _isatty */
#define ENTRADA_E_TECLADO() _isatty(_fileno(stdin))
#else
#include <termios.h> /* desligar o eco do terminal */
#include <unistd.h>  /* isatty */
#define ENTRADA_E_TECLADO() isatty(fileno(stdin))
#endif

/* ------------------------------------------------------------------ */
/* Validacao e arquivo de contas                                       */
/* ------------------------------------------------------------------ */

int usuario_valido(const char *usuario)
{
    int i, n = (int)strlen(usuario);

    if (n == 0 || n > MAX_USUARIO)
        return 0;
    for (i = 0; i < n; i++) {
        unsigned char c = (unsigned char)usuario[i];
        if (!isalnum(c) && c != '_' && c != '-')
            return 0;
    }
    return 1;
}

/*
 * Procura o usuario no arquivo. Se achar, copia o hash guardado para
 * 'hash' (quando hash != NULL) e retorna 1.
 */
static int buscar_usuario(const char *usuario, char *hash)
{
    char linha[MAX_USUARIO + 32];
    FILE *f = fopen(ARQUIVO_USUARIOS, "r");

    if (f == NULL)
        return 0; /* nenhum usuario cadastrado ainda */

    while (fgets(linha, sizeof(linha), f) != NULL) {
        char *sep = strchr(linha, ':');
        if (sep == NULL)
            continue; /* linha mal formada: ignora */
        *sep = '\0';
        if (strcmp(linha, usuario) == 0) {
            if (hash != NULL) {
                strncpy(hash, sep + 1, 16);
                hash[16] = '\0';
            }
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

int usuario_existe(const char *usuario)
{
    return buscar_usuario(usuario, NULL);
}

/* ------------------------------------------------------------------ */
/* Transformacao da senha: FNV-1a de 64 bits                           */
/* ------------------------------------------------------------------ */

/*
 * FNV-1a: comeca com um valor fixo (offset basis) e, para cada byte,
 * faz  h = (h XOR byte) * primo_FNV  (a multiplicacao "da a volta" em
 * 2^64, ou seja, e aritmetica modular mod 2^64). O resultado espalha
 * bem pequenas mudancas: trocar uma letra muda o hash inteiro.
 */
static unsigned long long fnv1a_64(const char *s, unsigned long long h)
{
    while (*s != '\0') {
        h ^= (unsigned char)*s;
        h *= 1099511628211ULL; /* primo FNV de 64 bits */
        s++;
    }
    return h;
}

void transformar_senha(const char *usuario, const char *senha, char *saida)
{
    unsigned long long h = 14695981039346656037ULL; /* offset basis FNV */

    h = fnv1a_64(usuario, h); /* o nome funciona como sal */
    h = fnv1a_64(":", h);
    h = fnv1a_64(senha, h);
    sprintf(saida, "%016llx", h);
}

int salvar_usuario(const char *usuario, const char *senha)
{
    char hash[17];
    FILE *f = fopen(ARQUIVO_USUARIOS, "a"); /* "a" acrescenta no fim */

    if (f == NULL)
        return 0;
    transformar_senha(usuario, senha, hash);
    fprintf(f, "%s:%s\n", usuario, hash);
    fclose(f);
    return 1;
}

int conferir_senha(const char *usuario, const char *senha)
{
    char guardado[17], calculado[17];

    if (!buscar_usuario(usuario, guardado))
        return 0;
    /* Aplica a mesma transformacao e compara os hashes, nunca as senhas. */
    transformar_senha(usuario, senha, calculado);
    return strcmp(guardado, calculado) == 0;
}

/* ------------------------------------------------------------------ */
/* Leitura da senha sem eco                                            */
/* ------------------------------------------------------------------ */

int ler_senha(const char *mensagem, char *senha, int tam)
{
    printf("%s", mensagem);
    fflush(stdout);

    if (!ENTRADA_E_TECLADO()) {
        /* Entrada vem de arquivo/pipe: le a linha normalmente. */
        if (fgets(senha, tam, stdin) == NULL)
            return 0;
        senha[strcspn(senha, "\r\n")] = '\0';
        printf("\n");
        return 1;
    }

#ifdef _WIN32
    {
        int n = 0, c;
        while ((c = _getch()) != '\r' && c != '\n') {
            if (c == '\b') {            /* backspace apaga o ultimo */
                if (n > 0)
                    n--;
            } else if (c == 3) {        /* Ctrl+C */
                n = 0;
                break;
            } else if (n < tam - 1 && c >= 32) {
                senha[n++] = (char)c;
            }
        }
        senha[n] = '\0';
    }
#else
    {
        struct termios antigo, novo;
        tcgetattr(STDIN_FILENO, &antigo);
        novo = antigo;
        novo.c_lflag &= ~ECHO;          /* desliga o eco */
        tcsetattr(STDIN_FILENO, TCSANOW, &novo);
        if (fgets(senha, tam, stdin) == NULL)
            senha[0] = '\0';
        tcsetattr(STDIN_FILENO, TCSANOW, &antigo); /* religa o eco */
        senha[strcspn(senha, "\r\n")] = '\0';
    }
#endif
    printf("\n");
    return 1;
}

/* ------------------------------------------------------------------ */
/* Sessao                                                              */
/* ------------------------------------------------------------------ */

static char usuario_logado[MAX_USUARIO + 1] = "";

const char *sessao_usuario(void)
{
    return usuario_logado;
}

void sessao_iniciar(const char *usuario)
{
    strncpy(usuario_logado, usuario, MAX_USUARIO);
    usuario_logado[MAX_USUARIO] = '\0';
}

void sessao_encerrar(void)
{
    usuario_logado[0] = '\0';
}
