#!/bin/bash

DEBUG_CFLAGS="-Wall -g"
RELEASE_CFLAGS="-O3"
LDFLAGS="-Wall -g -lglfw -lGLEW -lGL -lassimp -lcglm -lm"

SOURCE="src/*.c"
OUTPUT="main"


cc $DEBUG_CFLAGS -o $OUTPUT $SOURCE $LDFLAGS
