#include "camera.h"
#include <math.h>

void camera_init(Camera* camera, vec3 position, vec3 up, float yaw, float pitch, float fov)
{
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

    camera->orientation[0] = cos(glm_rad(yaw)) * cos(glm_rad(pitch));
    camera->orientation[1] = sin(glm_rad(pitch));
    camera->orientation[2] = sin(glm_rad(yaw)) * cos(glm_rad(pitch));

    glm_vec3_normalize(camera->orientation);

    glm_vec3_cross(camera->orientation, camera->world_up, camera->right);
    glm_vec3_normalize(camera->right);

    glm_vec3_cross(camera->right, camera->orientation, camera->up);
    glm_vec3_normalize(camera->up);

    glm_mat4_identity(camera->model);
    glm_mat4_identity(camera->view);
    glm_mat4_identity(camera->projection);
}

void camera_update(Camera* camera, GLFWwindow* window, float delta_time)
{
    camera_process_input(camera, window, delta_time);
}

void camera_process_input(Camera* camera, GLFWwindow* window, float delta_time)
{
    float velocity = camera->movement_speed * delta_time;
    vec3 movement = {0.0f, 0.0f, 0.0f};
    
    switch (glfwGetKey(window, GLFW_KEY_W)) {
        case GLFW_PRESS:
            glm_vec3_scale(camera->front, velocity, movement);
            glm_vec3_add(camera->position, movement, camera->position);
            break;
        case GLFW_RELEASE:
            // Handle something here idk yet
            break;
        default:
            break;
    }

    switch (glfwGetKey(window, GLFW_KEY_S)) {
        case GLFW_PRESS:
            glm_vec3_scale(camera->front, velocity, movement);
            glm_vec3_sub(camera->position, movement, camera->position);
            break;
        case GLFW_RELEASE:
            
            break;
        default:
            break;
    }

    switch (glfwGetKey(window, GLFW_KEY_A)) {
        case GLFW_PRESS:
            glm_vec3_scale(camera->right, velocity, movement);
            glm_vec3_sub(camera->position, movement, camera->position);
            break;
        case GLFW_RELEASE:
            
            break;
        default:
            break;
    }

    switch (glfwGetKey(window, GLFW_KEY_D)) {
        case GLFW_PRESS:
            glm_vec3_scale(camera->right, velocity, movement);
            glm_vec3_add(camera->position, movement, camera->position);
            break;
        case GLFW_RELEASE:
            
            break;
        default:
            break;
    }
}

void camera_process_mouse(Camera* camera, double x_offset, double y_offset)
{
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

void camera_get_view_matrix(Camera* camera)
{
    vec3 center;
    glm_vec3_add(camera->position, camera->front, center);
    glm_lookat(camera->position, center, camera->up, camera->view);
}

void remove_translation_matrix(Camera *camera)
{
    mat3 view_without_translation_mat3;
    glm_mat4_pick3(camera->view, view_without_translation_mat3);

    glm_mat4_identity(camera->view);
    glm_mat4_ins3(view_without_translation_mat3, camera->view);
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
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
    double yoffset = center_y - ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    camera_process_mouse(camera, xoffset, yoffset);
    
    glfwSetCursorPos(window, center_x, center_y);
}

