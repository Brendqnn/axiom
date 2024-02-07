#include "window.h"


Window ax_window_create(int width, int height, const char *name)
{
    Window window = {0};

    window.width = width;
    window.height = height;
    window.last_frame = glfwGetTime();
    window.current_frame = 0.0;
    window.frame_delta = 0.0f;

    if (!glfwInit()) {
        fprintf(stderr, "%s", "Failed to initialize GLFW.\n");
        glfwTerminate();
        exit(0);
    }

    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);    
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 8);

    window.handle = glfwCreateWindow(width, height, name, NULL, NULL);
    if (window.handle == NULL) {
        fprintf(stderr, "%s", "Failed to create window.\n");
        glfwTerminate();
        exit(0);
    }

    glfwMakeContextCurrent(window.handle);

    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "%s", "Failed to initialize GLEW.\n");
        glfwTerminate();
        exit(0);
    }

    glfwSwapInterval(1);
    glViewport(0, 0, window.width, window.height);

    return window;
}

bool ax_window_should_close(Window *window)
{
    return glfwWindowShouldClose(window->handle);
}



