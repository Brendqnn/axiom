#!/bin/bash

DEBUG_CFLAGS="-Wall -g"
RELEASE_CFLAGS="-O3"
LDFLAGS="-lglfw -lGLEW -lGL -lassimp -lcglm -lm"

SOURCE="src/*.c src/imgui/*.cpp"
OUTPUT="main"

g++ $DEBUG_CFLAGS -o $OUTPUT $SOURCE $LDFLAGS
