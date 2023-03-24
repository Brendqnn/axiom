# Define the compiler and linker flags
CC = gcc
CFLAGS = -Wall -g -I C:\sdk\glfw-64\include
LDFLAGS = -L C:\sdk\glfw-64\lib-mingw-w64 -lglfw3 -lopengl32 -lgdi32 -luser32


# Define the source files and the target executable
SOURCES = main.c
EXECUTABLE = main.exe

# Define the build rules
all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(EXECUTABLE) $(LDFLAGS)

clean:
	rm -f $(EXECUTABLE)
