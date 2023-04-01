# Define the compiler and linker flags
CFLAGS = -Wall -g -I $(GLFW_INCLUDE_PATH) -I $(GLEW_INCLUDE_PATH) -I $(CGLM_INCLUDE_PATH)
LDFLAGS = -L $(GLFW_LIB_PATH) -L $(GLEW_LIB_PATH) -lglfw3 -lopengl32 -lgdi32 -luser32 -lglew32
RUN_COMMAND = start bin\debug\main

# Set environment variables for include and lib paths
GLFW_INCLUDE_PATH = C:\sdk\glfw-64\include
GLFW_LIB_PATH = C:\sdk\glfw-64\lib-mingw-w64
GLEW_INCLUDE_PATH = C:\sdk\glew-2.1.0\include
GLEW_LIB_PATH = C:\sdk\glew-2.1.0\lib\Release\x64
CGLM_INCLUDE_PATH = C:\sdk\cglm\include

SOURCES = src/*.c
EXECUTABLE = main.exe

all:
	gcc -c -std=c99 $(SOURCES) $(CFLAGS) && gcc *.o -o bin/debug/main -s $(LDFLAGS) && $(RUN_COMMAND)

clean:
	rm -f $(EXECUTABLE) *.o

