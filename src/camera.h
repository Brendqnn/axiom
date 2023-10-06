#ifndef CAMERA_H
#define CAMERA_H

#include <cglm/cglm.h>
#include <GLFW/glfw3.h>
#include "util/util.h"

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

    int render_distance;
} Camera;

void camera_init(Camera* camera, vec3 position, vec3 up, float yaw, float pitch, float fov, int render_distance);
void camera_update(Camera* camera, GLFWwindow* window, float delta_time);
void camera_process_input(Camera* camera, GLFWwindow* window, float delta_time);
void camera_process_mouse(Camera* camera, double x_offset, double y_offset);
void camera_get_view_matrix(Camera* camera, mat4 view_matrix);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

#endif  // CAMERA_H






