# Terminal PI2

Terminal simples feito em C para o Projeto Integrador II (Cibersegurança e Criptografia) da PUC-Campinas.

**Repositório:** https://github.com/lucasuchida11/terminal-pi2

**Integrantes:** Lucas Uchida e Yuri Lopes

## Como compilar e rodar

```bash
gcc -o terminal src/main.c src/parser.c src/comandos.c src/cesar.c src/usuarios.c
```

Se tiver o `make` instalado, basta rodar `make`.

```bash
./terminal
```

No Windows, rode `terminal.exe`.

## Comandos

| Comando | O que faz |
|---|---|
| `ajuda` ou `ajuda <comando>` | Mostra os comandos ou explica um deles |
| `sair` | Fecha o terminal |
| `ec <texto>` | Repete o texto na tela |
| `criar <arquivo> <texto>` | Cria um arquivo com o texto |
| `cat <arquivo>` | Mostra o que tem no arquivo |
| `cesar cifrar -t <texto> <chave>` | Cifra o texto com a cifra de César (chave de 0 a 25) |
| `cesar decifrar -t <texto> <chave>` | Decifra o texto |
| `cesar cifrar -f <arquivo> <chave>` | Cifra o arquivo e salva em `<arquivo>.cifrado` |
| `cesar decifrar -f <arquivo> <chave>` | Decifra o arquivo e salva em `<arquivo>.decifrado` |
| `registrar <usuario>` | Cria uma conta (pede a senha duas vezes) |
| `login <usuario>` | Entra na conta |
| `logout` | Sai da conta |
| `quem-sou` | Mostra quem está logado |

## Exemplo

```
> cesar cifrar -t ABC 3
DEF
> cesar decifrar -t DEF 3
ABC
> criar msg.txt Ataque ao amanhecer
arquivo msg.txt criado
> cesar cifrar -f msg.txt 3
resultado salvo em msg.txt.cifrado
> cat msg.txt.cifrado
Dwdtxh dr dpdqkhfhu
> sair
ate logo!
```

## Como funciona

O código fica na pasta `src/`, separado por assunto:

| Arquivo | O que tem |
|---|---|
| `main.c` | O laço principal que lê a linha e chama o comando |
| `parser.c` / `parser.h` | Funções para tratar a linha: `tirar_enter`, `tirar_aspas` e `depois_das_palavras` |
| `comandos.c` / `comandos.h` | Os comandos `ajuda`, `ec`, `criar` e `cat` |
| `cesar.c` / `cesar.h` | A cifra de César (`cesar_letra` e o comando `cesar`) |
| `usuarios.c` / `usuarios.h` | `registrar`, `login`, `logout` e `quem-sou` |

- O `main` fica num laço: mostra o `>`, lê a linha com `fgets`, pega a primeira palavra com `sscanf` e usa `if/else` para chamar a função do comando.
- Cada comando tem uma função própria (`cmd_ec`, `cmd_criar`, `cmd_cesar`...).
- **Cifra de César:** cada letra anda `k` posições no alfabeto. A conta é `(letra + k) % 26`, e o `% 26` faz o Z voltar para o A. Para decifrar, usamos `26 - k`. Maiúsculas e minúsculas são mantidas, e espaços, números e pontuação não mudam.
- **Senhas:** ficam em `usuarios.txt`, mas nunca como foram digitadas. A função `transformar_senha` transforma a senha em um número (`h = h * 33 + letra`), e é esse número que é salvo. No login, a senha digitada passa pela mesma conta, e os números são comparados.
