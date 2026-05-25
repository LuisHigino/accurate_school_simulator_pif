# ==========================================
# CONFIGURAÇÕES BÁSICAS
# ==========================================
EXEC = Accurate_School_Simulator
CC = gcc

# ==========================================
# PASTAS E ARQUIVOS
# ==========================================
# Diz ao compilador onde procurar os arquivos .h
INCLUDES = -IGAME/include

# Pega TODOS os arquivos .c dentro da pasta GAME/src
SRC = GAME/src/*.c

# ==========================================
# FLAGS
# ==========================================
CFLAGS = -Wall $(INCLUDES)

# Flags da Raylib (Para Linux. Se for Windows: -lraylib -lopengl32 -lgdi32 -lwinmm)
LDFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

# ==========================================
# REGRAS DE COMPILAÇÃO
# ==========================================
all: $(EXEC)

$(EXEC):
	$(CC) $(SRC) $(CFLAGS) -o $(EXEC) $(LDFLAGS)

clean:
	rm -f $(EXEC)

run: all
	WAYLAND_DISPLAY="" ./Accurate_School_Simulator
