#ifndef UTIL_H
#define UTIL_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define PI 3.14159265358979323846
#define MOVEMENT_SPEED 20.0f
#define CAMERA_SENSITIVITY 1.0f
#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080
#define CAMERA_FOV 90.0f
#define ARRAY_LEN(xs) sizeof(xs)/sizeof(xs[0])

double calculate_fps(double previous_time);


#endif // UTIL_H

