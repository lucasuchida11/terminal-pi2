# Como trabalhar no projeto

Repositório: https://github.com/lucasuchida11/terminal-pi2

## 1. Baixar o projeto

```bash
git clone https://github.com/lucasuchida11/terminal-pi2.git
cd terminal-pi2
```

## 2. Compilar e testar

```bash
gcc terminal.c -o terminal
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
git add terminal.c
git commit -m "O que voce fez"
git push -u origin nome-da-tarefa
```

Depois, no site do GitHub, clique em **Compare & pull request** para juntar com a `main`.

## Para adicionar um comando novo

1. Crie uma função `cmd_nome(char linha[])` em `terminal.c`.
2. Coloque um `else if (strcmp(comando, "nome") == 0) cmd_nome(linha);` no `main`.
3. Acrescente o nome, a descrição e o uso nas listas `comandos`, `descricoes` e `usos`, e some 1 em `total_comandos`.
