#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include "noise1234.h"

#define CAMERA_SENSITIVITY 1.0f
#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080
#define CAMERA_FOV 90.0f
#define ARRAY_LEN(xs) sizeof(xs)/sizeof(xs[0])
#define TERRAIN_HEIGHT 100
#define TERRAIN_WIDTH 100
#define TERRAIN_SCALE 1000.0f

double ax_calculate_fps();
void ax_enable_vsync();
void ax_disable_vsync();

#endif // UTIL_H

