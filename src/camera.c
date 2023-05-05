#include "camera.h"
#include <GLFW/glfw3.h>
#include <math.h>

#define PI 3.14159265358979323846
#define CAMERA_SPEED 2.5f
#define CAMERA_SENSITIVITY 0.1f
#define CAMERA_FOV 45.0f

void camera_init(Camera* camera, vec3 position, vec3 up, float yaw, float pitch, float fov) {
    glm_vec3_copy(position, camera->position);
    glm_vec3_copy(up, camera->world_up);
   
    camera->pitch = pitch;
    camera->fov = fov;
    camera->movement_speed = CAMERA_SPEED;
    camera->mouse_sens = CAMERA_SENSITIVITY;

    vec3 front;
    front[0] = cos(glm_rad(yaw)) * cos(glm_rad(pitch));
    front[1] = sin(glm_rad(pitch));
    front[2] = sin(glm_rad(yaw)) * cos(glm_rad(pitch));
    glm_vec3_normalize_to(front, camera->front);

    glm_vec3_cross(camera->front, camera->world_up, camera->right);
    glm_vec3_normalize(camera->right);

    glm_vec3_cross(camera->right, camera->front, camera->up);
    glm_vec3_normalize(camera->up);
}

void camera_get_view_matrix(Camera* camera, mat4 view_matrix) {
    vec3 center;
    glm_vec3_add(camera->position, camera->front, center);
    glm_lookat(camera->position, center, camera->up, view_matrix);
}

void camera_process_input(Camera* camera, GLFWwindow* window, float delta_time) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        vec3 movement;
        glm_vec3_scale(camera->front, camera->movement_speed * delta_time, movement);
        glm_vec3_add(camera->position, movement, camera->position);
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        vec3 movement;
        glm_vec3_scale(camera->front, camera->movement_speed * delta_time, movement);
        glm_vec3_sub(camera->position, movement, camera->position);
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        vec3 movement;
        glm_vec3_scale(camera->right, camera->movement_speed * delta_time, movement);
        glm_vec3_sub(camera->position, movement, camera->position);
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        vec3 movement;
        glm_vec3_scale(camera->right, camera->movement_speed * delta_time, movement);
        glm_vec3_add(camera->position, movement, camera->position);
    }
}



