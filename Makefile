CC = gcc
CFLAGS = -Wall -Wextra -std=c99
SRC = src/main.c src/parser.c src/comandos.c src/cesar.c

terminal: $(SRC)
	$(CC) $(CFLAGS) -o terminal $(SRC)

clean:
	rm -f terminal terminal.exe
