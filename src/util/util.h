#include <stdio.h>
#include <stdlib.h>

void calculate_fps(double frame_time) {
    static double previous_time = 0.0;
    static int frame_count = 0;

    double current_time = glfwGetTime();
    double elapsed_time = current_time - previous_time;
    frame_count++;
    
    if (elapsed_time >= 1.0) {
        double fps = frame_count / elapsed_time;
        printf("FPS: %.00f\r", fps);
        frame_count = 0;
        previous_time = current_time;
        fflush(stdout);
    }
}
