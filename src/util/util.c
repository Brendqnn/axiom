#include "util.h"

double calculate_fps()
{
    static double last_display_time = 0.0;  // Initialize to 0.0
    static double fps = 0.0;
    static int frame_count = 0;

    if (last_display_time == 0.0) {
        last_display_time = glfwGetTime();
    }

    double current_time = glfwGetTime();
    double elapsed_time = current_time - last_display_time;
    frame_count++;

    if (elapsed_time >= 0.5) {
        fps = frame_count / elapsed_time;
        frame_count = 0;
        last_display_time = current_time;
    }

    return fps;
}

void enable_vsync()
{
    glfwSwapInterval(1);
}

void disable_vsync()
{
    glfwSwapBuffers(0);
}
