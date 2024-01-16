#!/bin/bash

DEBUG_CFLAGS="-Wall -g"
RELEASE_CFLAGS="-O3"
LDFLAGS="-lglfw -lGLEW -lGL -lassimp -lcglm -lm"

SOURCE_C=$(find src -name '*.c')
SOURCE_CPP=$(find src -name '*.cpp')
OUTPUT="bin/main"

g++ $DEBUG_CFLAGS -o $OUTPUT $SOURCE_C $SOURCE_CPP $LDFLAGS
