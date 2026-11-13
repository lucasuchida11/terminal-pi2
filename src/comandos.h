#ifndef COMANDOS_H
#define COMANDOS_H

/* Valores de retorno de um comando */
#define CONTINUAR 0
#define ENCERRAR  1

/* Todo comando recebe os argumentos da linha, igual ao main de um programa C. */
typedef int (*funcao_comando)(int argc, char *argv[]);

/*
 * Tabela de comandos: cada entrada tem o nome, uma descricao curta (para
 * "ajuda"), a sintaxe, um exemplo e a funcao que executa o comando.
 * Para adicionar um comando novo (ex.: login na A2), basta escrever a
 * funcao e acrescentar uma linha na tabela em comandos.c.
 */
typedef struct {
    const char *nome;
    const char *descricao;
    const char *sintaxe;
    const char *exemplo;
    funcao_comando executar;
} comando;

/* Procura o comando pelo nome e executa. Retorna CONTINUAR ou ENCERRAR. */
int executar_comando(int argc, char *argv[]);

#endif
