# Como trabalhar no projeto

Repositório: https://github.com/lucasuchida11/terminal-pi2

## 1. Baixar o projeto

```bash
git clone https://github.com/lucasuchida11/terminal-pi2.git
cd terminal-pi2
```

## 2. Compilar e testar

```bash
gcc -o terminal src/main.c src/parser.c src/comandos.c src/cesar.c src/usuarios.c
./terminal
```

## 3. Criar uma branch para a sua tarefa

```bash
git checkout main
git pull
git checkout -b nome-da-tarefa
```

## 4. Salvar e enviar

```bash
git add src
git commit -m "O que voce fez"
git push -u origin nome-da-tarefa
```

Depois, no site do GitHub, clique em **Compare & pull request** para juntar com a `main`.

## Para adicionar um comando novo

1. Crie a função `cmd_nome(char linha[])` no arquivo `.c` do assunto (ou em um arquivo novo) e coloque a declaração dela no `.h` correspondente.
2. No `src/main.c`, coloque um `else if (strcmp(comando, "nome") == 0) cmd_nome(linha);`.
3. No `src/comandos.c`, acrescente o nome, a descrição e o uso nas listas `comandos`, `descricoes` e `usos`, e some 1 em `total_comandos`.
4. Se criou um arquivo `.c` novo, coloque-o também no comando do `gcc` e no `Makefile`.
