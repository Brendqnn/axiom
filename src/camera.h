#ifndef CAMERA_H
#define CAMERA_H

#include <cglm/cglm.h>
#include <GLFW/glfw3.h>

typedef struct Camera {
    vec3 position;
    vec3 front;
    vec3 up;
    vec3 right;
    vec3 world_up;
    float yaw;
    float pitch;
    float movement_speed;
    float mouse_sens;
    float fov;
} Camera;

void camera_init(Camera* camera, vec3 position, vec3 up, float yaw, float pitch, float fov);
void camera_get_view_matrix(Camera* camera, mat4 view_matrix);
void camera_process_input(Camera* camera, GLFWwindow* window, float delta_time);
void camera_process_mouse(Camera* camera, float x_offset, float y_offset, bool constrain_pitch);
void camera_process_scroll(Camera* camera, float y_offset);

#endif

