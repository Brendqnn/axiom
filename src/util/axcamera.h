#ifndef AXCAMERA_H
#define AXCAMERA_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#include "axutil.h"
#include "../gfx/axwindow.h"


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

    float view_distance;
} AXCamera;

void ax_init_camera(AXCamera *camera, vec3 position, vec3 up, float yaw, float pitch, float fov);
void ax_update_camera(AXCamera *camera, AXWindow *window);
void ax_process_camera_input(AXCamera *camera, AXWindow *window, float delta_time);
void ax_process_mouse_input(AXCamera *camera, double x_offset, double y_offset);
void ax_get_camera_view_matrix(AXCamera *camera);
void ax_cursor_position_callback(GLFWwindow *window, double xpos, double ypos);
void ax_cursor_position_callback2(GLFWwindow *window, double xpos, double ypos);
void ax_remove_translation_matrix(AXCamera *camera);
void ax_set_cursor_pos_callback(AXCamera *camera, AXWindow *window);
void ax_enable_cursor_capture(AXCamera *camera, AXWindow *window);
void ax_disable_cursor_capture(GLFWwindow *window);
void ax_set_camera_view(AXCamera *camera);

#endif  // CAMERA_H
