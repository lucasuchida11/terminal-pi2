#ifndef USUARIOS_H
#define USUARIOS_H

#define MAX_USUARIO 32
#define MAX_SENHA   64
#define MIN_SENHA   4

/* Arquivo onde as contas ficam guardadas, uma por linha: usuario:hash */
#define ARQUIVO_USUARIOS "usuarios.dat"

/*
 * Nome de usuario valido: 1 a MAX_USUARIO caracteres, so letras, digitos,
 * '_' ou '-'. Isso impede, por exemplo, o ':' que separa os campos no arquivo.
 */
int usuario_valido(const char *usuario);

/* Retorna 1 se o usuario ja tem conta no arquivo. */
int usuario_existe(const char *usuario);

/*
 * Transformacao da senha (definida pelo grupo). A senha nunca e gravada em
 * texto puro: guardamos o hash FNV-1a de 64 bits de "usuario:senha", em
 * hexadecimal (16 caracteres). O nome do usuario entra como "sal", entao
 * duas pessoas com a mesma senha ficam com hashes diferentes.
 * 'saida' precisa ter pelo menos 17 posicoes.
 */
void transformar_senha(const char *usuario, const char *senha, char *saida);

/* Grava uma conta nova. Retorna 1 se deu certo. */
int salvar_usuario(const char *usuario, const char *senha);

/* Confere a senha de um usuario existente. Retorna 1 se estiver correta. */
int conferir_senha(const char *usuario, const char *senha);

/*
 * Le uma senha do teclado sem mostrar o que e digitado. Quando a entrada nao
 * e um teclado (ex.: testes com arquivo), le a linha normalmente.
 * Retorna 1 se leu algo, 0 em fim de entrada.
 */
int ler_senha(const char *mensagem, char *senha, int tam);

/* Sessao: quem esta logado no momento ("" = ninguem). */
const char *sessao_usuario(void);
void sessao_iniciar(const char *usuario);
void sessao_encerrar(void);

#endif
