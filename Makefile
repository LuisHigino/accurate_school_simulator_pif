EXEC = accurateschoolsimulator

# Compilador e flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -I./GAME -I/usr/local/include -g
LDFLAGS = -L/usr/local/lib -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

# Encontra todos os arquivos .c da pasta do jogo
SRC = $(wildcard GAME/*.c)
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
run: $(EXEC)
	./$(EXEC)