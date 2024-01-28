UNAME_S = $(shell uname -s)

CC = clang
CFLAGS = -std=c11 -O3 -g -Wall -Wextra -Wpedantic
CFLAGS += -Ilib/glfw/include -Ilib/glew/include -Ilib/cglm/include -IC:/sdk/assimp/include
LDFLAGS = -Llib/glfw/lib -Llib/glew/lib -Llib/cglm/lib -LC:/sdk/assimp/lib
LDLIBS = -lglfw3 -lopengl32 -lgdi32 -luser32 -lglew32 -lassimp-vc143-mtd  

SRC  = $(wildcard src/**/*.c) $(wildcard src/*.c) $(wildcard src/**/**/*.c) $(wildcard src/**/**/**/*.c)
OBJ  = $(SRC:.c=.o)
BIN = bin

.PHONY: all clean

all: game

game: $(BIN)/main.exe

$(BIN)/main.exe: $(OBJ)
	gcc $(OBJ) -o $(BIN)/main.exe $(LDFLAGS)

bin/debug/%.o: src/%.c
	gcc -c -std=c11 $(CFLAGS) $< -o $@

clean:
	 del /Q bin\*.o $(BIN)/main.exe
