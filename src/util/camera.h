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
    
    double last_x;
    double last_y;
} Camera;

/**
 * Initialize the camera with specified parameters.
 *
 * @param camera   The Camera structure to initialize.
 * @param position The initial position of the camera.
 * @param up       The up vector of the camera.
 * @param yaw      The initial yaw angle.
 * @param pitch    The initial pitch angle.
 * @param fov      The field of view angle.
 */
void camera_init(Camera *camera, vec3 position, vec3 up, float yaw, float pitch, float fov);

/**
 * Update the camera's state.
 *
 * @param camera      The Camera structure to update.
 * @param window      The GLFW window.
 * @param delta_time  The time elapsed since the last frame.
 */
void camera_update(Camera *camera, Window *window);

/**
 * Process input events to control the camera.
 *
 * @param camera      The Camera structure to control.
 * @param window      The GLFW window.
 * @param delta_time  The time elapsed since the last frame.
 */
void camera_process_input(Camera *camera, Window *window, float delta_time);

/**
 * Process mouse movements to update the camera's orientation.
 *
 * @param camera     The Camera structure to update.
 * @param x_offset   The change in the x-coordinate of the mouse.
 * @param y_offset   The change in the y-coordinate of the mouse.
 */
void camera_process_mouse(Camera *camera, double x_offset, double y_offset);

/**
 * Get the view matrix of the camera.
 *
 * @param camera      The Camera structure.
 * @param view_matrix  Output parameter for the view matrix.
 */
void camera_get_view_matrix(Camera *camera);

/**
 * Callback for handling cursor position changes.
 *
 * @param window The GLFW window.
 * @param xpos   The x-coordinate of the cursor.
 * @param ypos   The y-coordinate of the cursor.
 */
void cursor_position_callback(GLFWwindow *window, double xpos, double ypos);

/**
 * Remove the translation from view matrix.
 *
 * @param input_matrix   The input matrix
 * @param output_matrix  The output matrix without translation
 */
void remove_translation_matrix(Camera *camera);

void set_cursor_pos(Camera *camera, Window *window);

void disable_cursor_pos(GLFWwindow *window);

#endif  // CAMERA_H
