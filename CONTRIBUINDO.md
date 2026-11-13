# Como participar do projeto

Guia passo a passo para quem vai entrar no repositório e trabalhar em uma branch própria.

## 1. Preparar a máquina (uma vez só)

1. Instale o **Git**: https://git-scm.com/downloads
2. Instale o **gcc**. No Windows, use o MinGW-w64 (por exemplo, via https://www.msys2.org ou `winget install BrechtSanders.WinLibs.POSIX.UCRT`).
3. Configure seu nome e e-mail. Use o **mesmo e-mail da sua conta do GitHub**, para os commits contarem no seu perfil:

```bash
git config --global user.name "Seu Nome"
git config --global user.email "seu-email-do-github@exemplo.com"
```

## 2. Aceitar o convite

Você vai receber um convite de colaborador por e-mail ou em https://github.com/notifications. Aceite o convite; sem isso, você não consegue enviar código.

## 3. Baixar o repositório

```bash
git clone https://github.com/lucasuchida11/terminal-pi2.git
cd terminal-pi2
```

## 4. Compilar e testar

```bash
gcc -Wall -Wextra -std=c99 -o terminal src/main.c src/parser.c src/comandos.c src/cesar.c src/usuarios.c
./terminal
```

Digite `ajuda` para ver os comandos e `sair` para fechar.

## 5. Criar a sua branch

Nunca trabalhe direto na `main`. Crie uma branch com o nome da tarefa:

```bash
git checkout main
git pull
git checkout -b usuarios-login
```

## 6. Trabalhar e salvar (commits)

Faça commits pequenos e frequentes, porque o professor avalia o histórico:

```bash
git add src/comandos.c
git commit -m "Adiciona comando registrar"
```

Para adicionar um comando novo, escreva a função `cmd_xxx(int argc, char *argv[])` em `src/comandos.c` e acrescente uma linha na tabela `COMANDOS`. O `ajuda` lista o comando automaticamente.

## 7. Enviar para o GitHub

```bash
git push -u origin usuarios-login
```

Depois, entre no repositório pelo site, clique em **Compare & pull request** e abra um Pull Request para a `main`. O outro integrante revisa e faz o merge.

## 8. Antes de começar algo novo

Atualize sua cópia da `main` para não trabalhar em cima de código velho:

```bash
git checkout main
git pull
git checkout -b nome-da-proxima-tarefa
```

## Divisão sugerida para a A2

| Tarefa | Branch sugerida |
|---|---|
| `registrar`, `login`, `logout`, `quem-sou` | `usuarios-login` |
| Segunda cifra (afim, Hill ou substituição) | `segunda-cifra` |
| Relatório final e justificativa da cifra | `relatorio-a2` |

**Lembrete:** a defesa na banca é individual, então cada um precisa entender o código inteiro, inclusive a parte que o outro fez.
