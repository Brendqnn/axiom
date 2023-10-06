DEBUG_FLAGS = -Wall -Icglm/include -g
RELEASE_FLAGS = -O3 -Icglm/include
LDFLAGS = -lglfw -lGLEW -lGL -lm -lassimp

DEBUG_RUN_COMMAND = bin/debug/main.exe
RELEASE_RUN_COMMAND = bin/release/main.exe

SOURCES = $(wildcard src/*.c)
DEBUG_OBJECTS = $(patsubst src/%.c, bin/debug/%.o, $(SOURCES))
RELEASE_OBJECTS = $(patsubst src/%.c, bin/release/%.o, $(SOURCES))

DEBUG_EXECUTABLE = bin/debug/main.exe
RELEASE_EXECUTABLE = bin/release/main.exe

all: debug release

debug: $(DEBUG_EXECUTABLE)

$(DEBUG_EXECUTABLE): $(DEBUG_OBJECTS)
	gcc $(DEBUG_OBJECTS) -o $(DEBUG_EXECUTABLE) $(LDFLAGS)

bin/debug/%.o: src/%.c
	gcc -c -std=c11 $(DEBUG_FLAGS) $< -o $@

release: $(RELEASE_EXECUTABLE)

$(RELEASE_EXECUTABLE): $(RELEASE_OBJECTS)
	gcc $(RELEASE_OBJECTS) -o $(RELEASE_EXECUTABLE) $(LDFLAGS)

bin/release/%.o: src/%.c
	gcc -c -std=c11 $(RELEASE_FLAGS) $< -o $@

clean:
	rm -f $(DEBUG_EXECUTABLE) $(DEBUG_OBJECTS) $(RELEASE_EXECUTABLE) $(RELEASE_OBJECTS)

run-debug: $(DEBUG_EXECUTABLE)
	$(DEBUG_RUN_COMMAND)

run-release: $(RELEASE_EXECUTABLE)
	$(RELEASE_RUN_COMMAND)

