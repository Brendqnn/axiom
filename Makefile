DEBUG_FLAGS = -Wall -I$(CGLM_INCLUDE_PATH) -I$(GLFW_INCLUDE_PATH) -I$(GLEW_INCLUDE_PATH) -IC:/sdk/assimp/include -g
RELEASE_FLAGS = -O3 -I$(CGLM_INCLUDE_PATH) -I$(GLFW_INCLUDE_PATH) -I$(GLEW_INCLUDE_PATH) -IC:/sdk/assimp/include
LDFLAGS = -L$(GLFW_LIB_PATH) -L$(GLEW_LIB_PATH) -LC:/sdk/Assimp/lib/Debug -lglfw3 -lopengl32 -lgdi32 -luser32 -lglew32 -lassimp-vc143-mtd

DEBUG_RUN_COMMAND = bin/debug/main.exe
RELEASE_RUN_COMMAND = bin/release/main.exe

GLFW_INCLUDE_PATH = C:/sdk/glfw-64/include
GLFW_LIB_PATH = C:/sdk/glfw-64/lib-mingw-w64
GLEW_INCLUDE_PATH = C:/sdk/glew-2.1.0/include
GLEW_LIB_PATH = C:/sdk/glew-2.1.0/lib/Release/x64
CGLM_INCLUDE_PATH = C:/sdk/cglm/include

SOURCES = $(wildcard src/*.c)
DEBUG_OBJECTS = $(patsubst src/%.c, bin/debug/%.o, $(SOURCES))
RELEASE_OBJECTS = $(patsubst src/%.c, bin/release/%.o, $(SOURCES))

DEBUG_EXECUTABLE = bin\debug\main.exe
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
	 del /Q bin\debug\*.o $(DEBUG_EXECUTABLE) 

run-debug: $(DEBUG_EXECUTABLE)
	$(DEBUG_RUN_COMMAND)

run-release: $(RELEASE_EXECUTABLE)
	$(RELEASE_RUN_COMMAND)
