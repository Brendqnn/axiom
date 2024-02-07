#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <noise1234.h>

#include "util/camera.h"
#include "gfx/window.h"
#include "util/util.h"
#include "models/model.h"
#include "gfx/shader.h"
#include "gfx/skybox.h"


int main(void)
{
    Window window = ax_window_create(WINDOW_WIDTH, WINDOW_HEIGHT, "Axiom v1.11");

    const char* skybox_face_paths[6] = { 
        "res/skybox/right.jpg",
        "res/skybox/left.jpg",
        "res/skybox/top.jpg",
        "res/skybox/bottom.jpg",
        "res/skybox/front.jpg",
        "res/skybox/back.jpg"
    };

    Model model_t = load_model("res/tree/Prunus_Pendula.gltf");

    Shader shader = shader_create("res/shaders/default.vert", "res/shaders/default.frag");
    //Shader test = shader_create("res/shaders/terrain.vert", "res/shaders/terrain.frag");

    Skybox skybox = skybox_init(skybox_face_paths);

    Camera camera;
    vec3 position = {0.0f, 0.0f, 50.0f};
    vec3 up = {0.0f, 10.0f, 0.0f};
    
    camera_init(&camera, position, up, 0.0f, 0.0f, CAMERA_FOV);

    glEnable(GL_DEPTH_TEST);

    set_cursor_pos(&camera, &window);
    
    while (!ax_window_should_close(&window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        camera_update(&camera, &window); 

        if (glfwGetKey(window.handle, GLFW_KEY_R) == GLFW_PRESS) {
            disable_cursor_pos(window.handle);
        }

        if (glfwGetKey(window.handle, GLFW_KEY_T) == GLFW_PRESS) {
            set_cursor_pos(&camera, &window);
        }

        if (glfwGetKey(window.handle, GLFW_KEY_X) == GLFW_PRESS && glfwGetKey(window.handle, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
            free_model(&model_t);
            exit(1);
        }
        
        glm_mat4_identity(model_t.matrix);

        draw_model(model_t, &shader);
        shader_uniform_mat4(&shader, "view", camera.view);
        shader_uniform_mat4(&shader, "projection", camera.projection);
        shader_uniform_mat4(&shader, "model", model_t.matrix); 

        skybox_render(skybox, &camera);

        glfwSwapBuffers(window.handle);
        glfwPollEvents();
    }

    free_model(&model_t);
    glfwTerminate();
    return 0;
}
