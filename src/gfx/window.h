#ifndef WINDOW_H
#define WINDOW_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "../util/util.h"


typedef struct {
    GLFWwindow *handle;
    int width, height;

    double last_frame, current_frame;
    float frame_delta;
} Window;

Window ax_window_create(int width, int height, const char *name);
bool ax_window_should_close(Window *window);

#endif // WINDOW_H
