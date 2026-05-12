EXEC = accurateschoolsimulator

# Compilador e flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -I/usr/local/include -I../GAME/include -g
LDFLAGS = -L/usr/local/lib -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

# Encontra todos os arquivos .c na pasta atual
# Include local .c files and the shared abelha implementation
SRC = $(wildcard *.c) ../abelha.c
# Substitui a extensão .c por .o para gerar os objetos
OBJ = $(SRC:.c=.o)

# Regra principal (o que roda quando você digita apenas 'make')
all: $(EXEC)

# Regra para compilar o executável
$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC) $(LDFLAGS)

# Regra para compilar os arquivos .o a partir dos .c
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Regra para limpar os arquivos compilados
clean:
	rm -f $(OBJ) $(EXEC)

# Regra para compilar e rodar o jogo de uma vez só
run: all
	./$(EXEC)