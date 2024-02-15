#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#include "util.h"
#include "../gfx/window.h"


typedef struct {
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

    mat4 view, model, projection;
    
    bool first_mouse;
    bool debug_ui;
    
    double last_x;
    double last_y;
} Camera;

void camera_init(Camera *camera, vec3 position, vec3 up, float yaw, float pitch, float fov);
void camera_update(Camera *camera, Window *window);
void camera_process_input(Camera *camera, Window *window, float delta_time);
void camera_process_mouse(Camera *camera, double x_offset, double y_offset);
void camera_get_view_matrix(Camera *camera);
void cursor_position_callback(GLFWwindow *window, double xpos, double ypos);
void remove_translation_matrix(Camera *camera);
void set_cursor_pos_callback(Camera *camera, Window *window);
void enable_cursor_capture(Camera *camera, Window *window);
void disable_cursor_capture(GLFWwindow *window);

#endif  // CAMERA_H
