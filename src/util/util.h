#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define MOVEMENT_SPEED 100.0f
#define CAMERA_SENSITIVITY 1.0f
#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080
#define CAMERA_FOV 90.0f
#define ARRAY_LEN(xs) sizeof(xs)/sizeof(xs[0])

double calculate_fps(double previous_time);

#endif // UTIL_H

