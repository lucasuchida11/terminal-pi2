# Relatório: Entrega A1

**Projeto Integrador II, Cibersegurança e Criptografia. Prof. Dr. Felix Carvalho Rodrigues, PUC-Campinas**

Integrantes: _(preencher)_

Repositório: https://github.com/lucasuchida11/terminal-pi2

## 1. Objetivo da etapa

Construir o núcleo de um terminal (interpretador de comandos) em C, com os comandos de uso geral `ajuda`, `sair`, `ec`, `criar` e `cat`, e a cifra de César implementada pelo grupo (`cesar cifrar` / `cesar decifrar`), aceitando entrada por texto (`-t`) ou por arquivo (`-f`).

## 2. Funcionamento do terminal

O programa segue o ciclo de um interpretador de comandos (`src/main.c`):

1. Mostra o prompt `> `.
2. Lê uma linha com `fgets` e remove a quebra de linha (`\n` e o `\r` do Windows).
3. Separa a linha em argumentos com `separar_argumentos` (`src/parser.c`). Espaços separam os argumentos, e um trecho entre aspas duplas conta como um argumento só. O resultado está no mesmo formato `argc`/`argv` que o `main` de um programa C recebe.
4. Procura `argv[0]` na tabela de comandos (`src/comandos.c`) e chama a função correspondente.
5. Repete até um comando devolver `ENCERRAR` (`sair`) ou até a entrada terminar.

### Organização em funções

- **Tabela de comandos.** Cada comando é uma `struct` com nome, descrição, sintaxe, exemplo e um ponteiro para a função que o executa. O despacho é uma busca nessa tabela, e o `ajuda` é gerado a partir dela. Assim, acrescentar um comando na A2 (por exemplo, `login`) não exige mexer no laço principal.
- **Interface padronizada.** Todo comando tem a assinatura `int cmd(int argc, char *argv[])`.
- **Tratamento de erros.** Cada comando valida os argumentos e informa o problema sem encerrar o terminal.

## 3. Comandos implementados

| Comando | Comportamento | Erros tratados |
|---|---|---|
| `ajuda [cmd]` | Lista os comandos ou mostra a sintaxe e um exemplo de um deles | comando desconhecido |
| `sair` | Mensagem de despedida e fim do programa | nenhum |
| `ec <texto>` | Imprime o texto | falta o texto |
| `criar <arq> <texto>` | Cria ou substitui o arquivo na pasta atual | nome vazio ou com barras, texto ausente, falha de escrita |
| `cat <arq>` | Exibe o conteúdo do arquivo | arquivo não encontrado |
| `cesar cifrar/decifrar -t/-f ... <k>` | Cifra ou decifra o texto na tela, ou o arquivo para `.cifrado`/`.decifrado` | deslocamento fora de 0–25 ou não numérico, arquivo inexistente, falta de argumentos, operação ou modo inválido |

## 4. Cifra de César

### Matemática

Numeramos o alfabeto como A=0, B=1, …, Z=25. Com a chave `k` (0 ≤ k ≤ 25):

- **Cifrar:** `c = (p + k) mod 26`
- **Decifrar:** `p = (c − k) mod 26`

O `mod 26` produz o deslocamento circular: depois do Z vem o A. Por exemplo, com k = 3, `Y` (24) vira (24 + 3) mod 26 = 1, ou seja, `B`.

Para evitar números negativos no C (onde `-1 % 26` dá `-1`), a decifração usa a identidade `(c − k) mod 26 = (c + (26 − k)) mod 26`. Na prática, decifrar com `k` é cifrar com `26 − k`, e o mesmo código serve para as duas operações.

### Implementação (`src/cesar.c`)

- `cesar_char`: se o caractere está entre `'A'` e `'Z'`, calcula `'A' + (c − 'A' + k) % 26`; o mesmo vale para minúsculas com `'a'`. Qualquer outro caractere (espaço, dígito, pontuação) volta inalterado. Isso preserva maiúsculas e minúsculas.
- `cesar_texto`: aplica `cesar_char` em cada posição da string.
- `cesar_arquivo`: lê o arquivo de entrada com `fgetc` e grava cada caractere transformado com `fputc`, o que funciona com arquivos de qualquer tamanho.

Nenhuma biblioteca de criptografia foi usada.

### Limites da cifra de César

Só existem 25 chaves úteis, então um ataque de força bruta testa todas em instantes. Além disso, a cifra preserva a frequência das letras (o `E` mais comum no texto claro vira a letra mais comum no texto cifrado), o que permite a análise de frequência. Ela serve para estudar aritmética modular, mas não protege dados de verdade.

## 5. Testes realizados

| Entrada | Saída obtida |
|---|---|
| `cesar cifrar -t ABC 3` | `DEF` |
| `cesar decifrar -t DEF 3` | `ABC` |
| `cesar cifrar -t "Ola, Mundo! XYZ 123" 3` | `Rod, Pxqgr! ABC 123` |
| `cesar cifrar -t xyz 25` | `wxy` |
| `cesar cifrar -t ABC 26` | erro: deslocamento inválido |
| `cesar cifrar -t ABC abc` | erro: deslocamento inválido |
| `criar m.txt Ataque ao Amanhecer, Zebra!` e depois `cesar cifrar -f m.txt 7` | `m.txt.cifrado` contém `Hahxbl hv Hthuoljly, Gliyh!` |
| `cesar decifrar -f m.txt.cifrado 7` | `m.txt.cifrado.decifrado` volta ao texto original |
| `criar a/b.txt oi` | erro: nome inválido |
| `cat nada.txt` | `arquivo nao encontrado` |
| `ajuda xyz` / `foo` | `comando desconhecido` |

## 6. Estado do projeto e próximos passos

A etapa A1 está completa: todos os comandos do catálogo marcados como A1 estão implementados e testados.

Para a A2 ainda faltam:

- os comandos `registrar`, `login`, `logout` e `quem-sou`, com a senha guardada transformada (nunca em texto puro);
- a segunda cifra (a definir pelo grupo), no mesmo formato `cifrar`/`decifrar` com `-t`/`-f`;
- o relatório final com a justificativa da cifra escolhida.
