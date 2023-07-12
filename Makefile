
CFLAGS = -Wall -O3 -g -Icglm/include
LDFLAGS = -lglfw -lGLEW -lGL -lm
RUN_COMMAND = bin/debug/main.exe

SOURCES = src/*.c
EXECUTABLE = main.exe

all:
	gcc -c -std=c11 $(SOURCES) $(CFLAGS) && \
	gcc *.o -o bin/debug/$(EXECUTABLE) -s $(LDFLAGS)

clean:
	rm -f bin/debug/$(EXECUTABLE) *.o

run:
	$(RUN_COMMAND)

