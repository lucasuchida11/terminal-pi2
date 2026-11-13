# Terminal PI2

Terminal simplificado em C. Projeto Integrador II, Cibersegurança e Criptografia (PUC-Campinas).

O programa mostra um prompt (`> `), lê a linha digitada, separa em argumentos e executa o comando. Ele repete isso até o usuário digitar `sair`.

## Como compilar

Você precisa do `gcc` (no Windows, MinGW; no Linux/macOS, o gcc do sistema).

```bash
gcc -Wall -Wextra -std=c99 -o terminal src/main.c src/parser.c src/comandos.c src/cesar.c
```

Se tiver `make` instalado, basta rodar:

```bash
make
```

## Como executar

```bash
./terminal        # Linux/macOS/Git Bash
terminal.exe      # Windows (cmd/PowerShell)
```

## Comandos (entrega A1)

| Comando | O que faz |
|---|---|
| `ajuda [comando]` | Lista os comandos ou mostra a sintaxe e um exemplo de um comando |
| `sair` | Encerra o programa |
| `ec <texto>` | Repete o texto na tela |
| `criar <arquivo> <texto>` | Cria (ou substitui) um arquivo com o texto; o nome não pode ter `/` nem `\` |
| `cat <arquivo>` | Mostra o conteúdo do arquivo |
| `cesar cifrar -t <texto> <k>` | Cifra o texto com César, deslocamento `k` de 0 a 25 |
| `cesar decifrar -t <texto> <k>` | Decifra o texto |
| `cesar cifrar -f <arquivo> <k>` | Cifra o arquivo e grava o resultado em `<arquivo>.cifrado` |
| `cesar decifrar -f <arquivo> <k>` | Decifra o arquivo e grava o resultado em `<arquivo>.decifrado` |

Um texto com espaços pode ir entre aspas (`ec "ola   mundo"`) ou solto (`ec ola mundo`).

## Exemplo de uso

```
> criar msg.txt Ataque ao amanhecer
arquivo msg.txt criado.
> cesar cifrar -f msg.txt 3
resultado gravado em msg.txt.cifrado
> cat msg.txt.cifrado
Dwdtxh dr dpdqkhfhu
> cesar cifrar -t ABC 3
DEF
> cesar decifrar -t DEF 3
ABC
> sair
Ate logo!
```

## Organização do código

```
src/
  main.c      laço principal: prompt -> leitura -> separação -> execução
  parser.c/h  separa a linha em argumentos (argc/argv), tratando aspas
  comandos.c/h tabela de comandos e implementação de cada um
  cesar.c/h   cifra de César (caractere, texto e arquivo)
docs/
  relatorio-A1.md
```

Para adicionar um comando, escreva a função `cmd_xxx(int argc, char *argv[])` em `comandos.c` e acrescente uma linha na tabela `COMANDOS`. O `ajuda` passa a listar o comando novo sozinho.

## Cifra de César

Numerando as letras de A=0 até Z=25 e usando a chave `k`:

- cifrar: `c = (p + k) mod 26`
- decifrar: `p = (c - k) mod 26`, que no código é feito como cifrar com `26 - k`

Maiúsculas e minúsculas são preservadas. Espaços, números e pontuação não mudam. Nenhuma biblioteca de criptografia é usada.

## Roadmap

- [x] A1: núcleo do terminal e cifra de César
- [ ] A2: `registrar`, `login`, `logout`, `quem-sou`
- [ ] A2: segunda cifra à escolha do grupo
