#include "camera.h"
#include <math.h>


void camera_init(Camera* camera, vec3 position, vec3 up, float yaw, float pitch, float fov) {
    glm_vec3_copy(position, camera->position);
    glm_vec3_copy(up, camera->world_up);

    camera->yaw = yaw;
    camera->pitch = pitch;
    camera->fov = fov;
    camera->movement_speed = CAMERA_SPEED;
    camera->mouse_sensitivity = CAMERA_SENSITIVITY;

    camera->first_mouse = true;
    camera->last_x = 0.0;
    camera->last_y = 0.0;

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

void camera_update(Camera* camera, GLFWwindow* window, float delta_time) {
    // Update camera position based on user input
    camera_process_input(camera, window, delta_time);

    // Update view matrix
    mat4 view_matrix;
    camera_get_view_matrix(camera, view_matrix);

    // Set the view matrix in your rendering code
    // glUniformMatrix4fv(view_matrix_location, 1, GL_FALSE, (float*)view_matrix);
}

void camera_process_input(Camera* camera, GLFWwindow* window, float delta_time) {
    float velocity = camera->movement_speed * delta_time;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        vec3 movement;
        glm_vec3_scale(camera->front, velocity, movement);
        glm_vec3_add(camera->position, movement, camera->position);
    }
	
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        vec3 movement;
        glm_vec3_scale(camera->front, velocity, movement);
        glm_vec3_sub(camera->position, movement, camera->position);
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        vec3 movement;
        glm_vec3_scale(camera->right, velocity, movement);
        glm_vec3_sub(camera->position, movement, camera->position);
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        vec3 movement;
        glm_vec3_scale(camera->right, velocity, movement);
        glm_vec3_add(camera->position, movement, camera->position);
    }

}

void camera_process_mouse(Camera* camera, double x_offset, double y_offset) {
    if (camera->first_mouse) {
        camera->last_x = x_offset;
        camera->last_y = y_offset;
        camera->first_mouse = false;
    }

    double x_offset_scaled = x_offset * camera->mouse_sensitivity;
    double y_offset_scaled = y_offset * camera->mouse_sensitivity;

    camera->yaw += x_offset_scaled;
    camera->pitch += y_offset_scaled;

    if (camera->pitch > 89.0)
        camera->pitch = 89.0;
    if (camera->pitch < -89.0)
        camera->pitch = -89.0;

    vec3 front;
    front[0] = cos(glm_rad(camera->yaw)) * cos(glm_rad(camera->pitch));
    front[1] = sin(glm_rad(camera->pitch));
    front[2] = sin(glm_rad(camera->yaw)) * cos(glm_rad(camera->pitch));
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

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    Camera* camera = (Camera*)glfwGetWindowUserPointer(window);

    static bool firstMouse = true;

    if (firstMouse) {
        xpos = WINDOW_WIDTH / 2;
        ypos = WINDOW_HEIGHT / 2;
        firstMouse = false;
    }

    double center_x = WINDOW_WIDTH / 2;
    double center_y = WINDOW_HEIGHT / 2;

    double xoffset = xpos - center_x;
    double yoffset = center_y - ypos;  // Reversed since y-coordinates range from bottom to top

    float sensitivity = 0.1f;  // Adjust this to control mouse sensitivity
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    // Update the camera's orientation based on the mouse movement
    camera_process_mouse(camera, xoffset, yoffset);

    // Set the cursor position back to the center of the window
    glfwSetCursorPos(window, center_x, center_y);
}

