#include "util.h"


double calculate_fps(double previous_time)
{
    static double last_display_time = glfwGetTime();
    static double fps = 0.0;
    static int frame_count = 0;

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
