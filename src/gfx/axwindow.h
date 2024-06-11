#ifndef AXWINDOW_H
#define AXWINDOW_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "../util/axutil.h"


typedef struct {
    GLFWwindow *handle;
    int width, height;
    
    double last_frame, current_frame;
    float frame_delta;
    bool fullscreen;
} AXWindow;

AXWindow ax_window_create(int width, int height, const char *name);
void ax_toggle_fullscreen(AXWindow *window);
bool ax_window_should_close(AXWindow *window);

#endif // AXWINDOW_H
