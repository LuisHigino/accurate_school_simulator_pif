EXEC = Accurate_School_Simulator
CC = gcc

INCLUDES = -IGAME/include

SRC = GAME/src/*.c

CFLAGS = -Wall $(INCLUDES)

LDFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

all: $(EXEC)

$(EXEC):
	$(CC) $(SRC) $(CFLAGS) -o $(EXEC) $(LDFLAGS)

clean:
	rm -f $(EXEC)

run: all
	WAYLAND_DISPLAY="" ./Accurate_School_Simulator
