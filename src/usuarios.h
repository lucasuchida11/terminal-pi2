#ifndef USUARIOS_H
#define USUARIOS_H

/* Nome de quem esta logado ("" = ninguem). Fica em usuarios.c. */
extern char usuario_logado[50];

unsigned long transformar_senha(char senha[]);
void cmd_registrar(char linha[]);
void cmd_login(char linha[]);
void cmd_logout();
void cmd_quem_sou();

#endif
