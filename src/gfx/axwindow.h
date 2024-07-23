#ifndef AXWINDOW_H
#define AXWINDOW_H

#include <GL/glew.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#include "../util/axutil.h"

typedef struct {
    GLFWwindow *handle;
    int width, height;
    
    double last_frame, current_frame;
    float frame_delta;
    
    bool fullscreen;
    bool debug_ui;
    bool vsync;
} AXWindow;

AXWindow ax_window_create(int width, int height, const char *name);
void ax_toggle_fullscreen(AXWindow *window);
bool ax_window_should_close(AXWindow *window);

#endif // AXWINDOW_H
