# Relatório: Entrega A1

Projeto Integrador II, Cibersegurança e Criptografia. PUC-Campinas. Prof. Dr. Felix Carvalho Rodrigues.

Integrantes: Lucas Uchida (RA 26007916) e Yuri Lopes (RA 26004182)

Repositório: https://github.com/lucasuchida11/terminal-pi2

## O que fizemos

Na A1 fizemos a base do terminal e a cifra de César, tudo no arquivo `terminal.c`:

- **Laço principal:** o `main` mostra o `>`, lê a linha com `fgets`, pega a primeira palavra com `sscanf` e chama a função do comando com `if/else`.
- **Comandos:** `ajuda`, `sair`, `ec`, `criar` e `cat`. Cada um mostra uma mensagem de erro quando falta alguma coisa.
- **Cifra de César:** `cesar cifrar` e `cesar decifrar`, com `-t` (texto) ou `-f` (arquivo) e uma chave de 0 a 25.

## Cifra de César

Numerando as letras de A = 0 até Z = 25:

- cifrar: `C = (P + k) mod 26`
- decifrar: `P = (C - k) mod 26`

No código, a função `cesar_letra` faz `'A' + (c - 'A' + k) % 26` para maiúsculas e o mesmo com `'a'` para minúsculas. Outros caracteres não mudam. Para decifrar, usamos a chave `26 - k`, porque voltar `k` posições é o mesmo que andar `26 - k`.

A cifra é fácil de quebrar: só tem 25 chaves e mantém a frequência das letras.

## Testes

| Entrada | Resultado |
|---|---|
| `cesar cifrar -t ABC 3` | `DEF` |
| `cesar decifrar -t DEF 3` | `ABC` |
| `cesar cifrar -t "Ola, Mundo! XYZ 123" 3` | `Rod, Pxqgr! ABC 123` |
| `cesar cifrar -t xyz 25` | `wxy` |
| `cesar cifrar -t ABC 26` | erro: chave inválida |
| `cesar cifrar -f m.txt 7` e depois `cesar decifrar -f m.txt.cifrado 7` | volta ao texto original |
| `criar a/b.txt oi` | erro: nome inválido |
| `cat nada.txt` | `arquivo nao encontrado` |
| `ajuda xyz` | `comando desconhecido` |
