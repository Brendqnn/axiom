#include <math.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "axcamera.h"


void ax_init_camera(AXCamera* camera, vec3 position, vec3 up, float yaw, float pitch, float fov)
{
    glm_vec3_copy(position, camera->position);
    glm_vec3_copy(up, camera->world_up);

    camera->yaw = yaw;
    camera->pitch = pitch;
    camera->fov = fov;
    camera->movement_speed = 500;
    camera->mouse_sensitivity = CAMERA_SENSITIVITY;
    camera->view_distance = 50000.0f;

    camera->first_mouse = true;
    camera->debug_ui = true;
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

    ax_set_camera_view(camera);
}

void ax_set_camera_view(AXCamera *camera)
{
    glm_perspective(glm_rad(camera->fov), (float)WINDOW_WIDTH/WINDOW_HEIGHT, 0.1f, camera->view_distance, camera->projection);
}

void ax_update_camera(AXCamera* camera, AXWindow *window)
{
    window->current_frame = glfwGetTime();
    window->frame_delta = window->current_frame - window->last_frame;
    window->last_frame = window->current_frame;

    ax_get_camera_view_matrix(camera);
    ax_process_camera_input(camera, window, window->frame_delta);
}

void ax_process_camera_input(AXCamera *camera, AXWindow *window, float delta_time)
{
    float velocity = camera->movement_speed * delta_time;
    vec3 movement = {0.0f, 0.0f, 0.0f}; // Initialize the movement vector.
    
    switch (glfwGetKey(window->handle, GLFW_KEY_W)) {
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

    switch (glfwGetKey(window->handle, GLFW_KEY_S)) {
        case GLFW_PRESS:
            glm_vec3_scale(camera->front, velocity, movement);
            glm_vec3_sub(camera->position, movement, camera->position);
            break;
        case GLFW_RELEASE:
            
            break;
        default:
            break;
    }

    switch (glfwGetKey(window->handle, GLFW_KEY_A)) {
        case GLFW_PRESS:
            glm_vec3_scale(camera->right, velocity, movement);
            glm_vec3_sub(camera->position, movement, camera->position);
            break;
        case GLFW_RELEASE:
            
            break;
        default:
            break;
    }

    switch (glfwGetKey(window->handle, GLFW_KEY_D)) {
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

void ax_process_mouse_input(AXCamera *camera, double x_offset, double y_offset)
{
    if (camera->first_mouse && !camera->debug_ui) {
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

void ax_get_camera_view_matrix(AXCamera *camera)
{
    vec3 center;
    glm_vec3_add(camera->position, camera->front, center);
    glm_lookat(camera->position, center, camera->up, camera->view);
}

void ax_remove_translation_matrix(AXCamera *camera)
{
    mat3 view_without_translation_mat3;
    glm_mat4_pick3(camera->view, view_without_translation_mat3);

    glm_mat4_identity(camera->view);
    glm_mat4_ins3(view_without_translation_mat3, camera->view);
}

void ax_cursor_position_callback(GLFWwindow *window, double xpos, double ypos)
{
    AXCamera* camera = (AXCamera*)glfwGetWindowUserPointer(window);
    static bool first_mouse = true;

    if (!camera->debug_ui) {
        if (first_mouse) {
            xpos = WINDOW_WIDTH/2;
            ypos = WINDOW_HEIGHT/2;
            first_mouse = false;
        }

        double center_x = WINDOW_WIDTH/2;
        double center_y = WINDOW_HEIGHT/2;
        double xoffset = xpos - center_x;
        double yoffset = center_y - ypos;

        float sensitivity = 0.1f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        ax_process_mouse_input(camera, xoffset, yoffset);

        glfwSetCursorPos(window, center_x, center_y);
    }

    // Switch mouse to capture imgui input
    ImGuiIO& io = ImGui::GetIO();
    io.MousePos = ImVec2((float)xpos + 10, (float)ypos + 30); // account for no fullscreen
}

void ax_cursor_position_callback2(GLFWwindow *window, double xpos, double ypos)
{
    AXCamera* camera = (AXCamera*)glfwGetWindowUserPointer(window);
    static bool first_mouse = true;

    if (!camera->debug_ui) {
        if (first_mouse) {
            xpos = WINDOW_WIDTH/2;
            ypos = WINDOW_HEIGHT/2;
            first_mouse = false;
        }

        double center_x = WINDOW_WIDTH/2;
        double center_y = WINDOW_HEIGHT/2;
        double xoffset = xpos - center_x;
        double yoffset = center_y - ypos;

        float sensitivity = 0.1f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        ax_process_mouse_input(camera, xoffset, yoffset);

        glfwSetCursorPos(window, center_x, center_y);
    }

    ImGuiIO& io = ImGui::GetIO();
    io.MousePos = ImVec2((float)xpos, (float)ypos); // account for fullscreen here
}

void ax_set_cursor_pos_callback(AXCamera *camera, AXWindow *window)
{
    glfwSetWindowUserPointer(window->handle, camera);
    if (window->fullscreen) {
        glfwSetCursorPosCallback(window->handle, ax_cursor_position_callback2); // there is definetly a better way to do this
    } else {
        glfwSetCursorPosCallback(window->handle, ax_cursor_position_callback);
    }
}

void ax_enable_cursor_capture(AXCamera *camera, AXWindow *window)
{
    glfwSetInputMode(window->handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void ax_disable_cursor_capture(GLFWwindow *window)
{
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}
