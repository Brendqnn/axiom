# Define the compiler and linker flags
CC = gcc -std=c99
CFLAGS = -Wall -g -I $(GLFW_INCLUDE_PATH) -I $(GLEW_INCLUDE_PATH)
LDFLAGS = -L $(GLFW_LIB_PATH) -L $(GLEW_LIB_PATH) -lglfw3 -lopengl32 -lgdi32 -luser32 -lglew32

# Set environment variables for include and lib paths
GLFW_INCLUDE_PATH = C:\sdk\glfw-64\include
GLFW_LIB_PATH = C:\sdk\glfw-64\lib-mingw-w64
GLEW_INCLUDE_PATH = C:\sdk\glew-2.1.0\include
GLEW_LIB_PATH = C:\sdk\glew-2.1.0\lib\Release\x64

SOURCES = main.c
EXECUTABLE = main.exe

all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(EXECUTABLE) $(LDFLAGS)

clean:
	rm -f $(EXECUTABLE)


