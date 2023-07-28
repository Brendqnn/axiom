#ifndef CAMERA_H
#define CAMERA_H

#include <cglm/cglm.h>
#include <GLFW/glfw3.h>

#define PI 3.14159265358979323846
#define CAMERA_SPEED 1.0f
#define CAMERA_SENSITIVITY 1.0f
#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080
#define CAMERA_FOV 60.0f

typedef struct Camera {
    vec3 position;
    vec3 front;
    vec3 up;
    vec3 right;
    vec3 world_up;
    float yaw;
    float pitch;
    float movement_speed;
    float mouse_sensitivity;
    float fov;
    bool first_mouse;
    double last_x;
    double last_y;
} Camera;

void camera_init(Camera* camera, vec3 position, vec3 up, float yaw, float pitch, float fov);
void camera_update(Camera* camera, GLFWwindow* window, float delta_time);
void camera_process_input(Camera* camera, GLFWwindow* window, float delta_time);
void camera_process_mouse(Camera* camera, double x_offset, double y_offset);
void camera_get_view_matrix(Camera* camera, mat4 view_matrix);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

#endif  // CAMERA_H






