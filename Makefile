UNAME_S = $(shell uname -s)

CC=g++
CFLAGS=-Wall -Wextra -Wpedantic -O3 -Ilib/glfw/include -Ilib/glew/include -Ilib/cglm/include -Ilib/assimp/include -Ilib/noise -Ilib/imgui
LDFLAGS=-Llib/glfw -Llib/glew -Llib/assimp -lglfw3 -lopengl32 -lgdi32 -luser32 -lglew32 -lassimp-vc143-mt lib/noise/libnoise.a lib/imgui/libimgui.a

SRC  = $(wildcard src/**/*.c) $(wildcard src/*.c) $(wildcard src/**/**/*.c) $(wildcard src/**/**/**/*.c) 
OBJ  = $(SRC:.c=.o)
BIN = bin

.PHONY: all clean

all: libs game

libs:
	cd lib/noise && make
	cd lib/imgui && make

dirs:
	mkdir -p .\$(BIN)

run: all
	$(BIN)/Axiom

game: $(OBJ)
	$(CC) -o $(BIN)/Axiom $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	del /Q bin\*.exe src\gfx\*.o src\models\*.o src\util\*.o src\main.o lib\imgui\*.o lib\noise\*.o


