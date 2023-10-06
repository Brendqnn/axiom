#define PI 3.14159265358979323846
#define CAMERA_SPEED 1.0f
#define CAMERA_SENSITIVITY 0.5f
#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080
#define CAMERA_FOV 90.0f

#define ARRAY_LEN(xs) sizeof(xs)/sizeof(xs[0])

void calculate_fps() {
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
