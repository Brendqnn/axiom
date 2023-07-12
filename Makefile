# Define the compiler and linker flags
CFLAGS = -Wall -g -Icglm/include
LDFLAGS = -lglfw -lGLEW -lGL -lm
RUN_COMMAND = bin/debug/main.exe

# Set environment variables for include and lib paths
SOURCES = src/*.c
EXECUTABLE = main.exe

all:
	gcc -c -std=c11 $(SOURCES) $(CFLAGS) && \
	gcc *.o -o bin/debug/$(EXECUTABLE) -s $(LDFLAGS)

clean:
	rm -f bin/debug/$(EXECUTABLE) *.o

run:
	$(RUN_COMMAND)

