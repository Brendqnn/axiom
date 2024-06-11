#include "axskybox.h"

// TODO: 

AXSkybox ax_init_skybox(const char *skybox_faces[])
{
    AXSkybox skybox;

    float vertices[] = { 
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    skybox.vao = ax_create_vao();
    skybox.vbo = ax_create_vbo(GL_ARRAY_BUFFER, false);
    
    ax_bind_vao(skybox.vao);
    ax_bind_vbo_buffer(skybox.vbo, vertices, sizeof(vertices));
    ax_link_attrib(skybox.vao, skybox.vbo, 0, 3, GL_FLOAT, 3 * sizeof(float), 0);

    skybox.cubemap = ax_load_cubemap_textures(skybox_faces);
    
    skybox.shader = ax_create_shader("res/shaders/skybox.vert", "res/shaders/skybox.frag");
    
    return skybox;
}

void ax_render_skybox(AXSkybox skybox, AXCamera *camera)
{
    ax_remove_translation_matrix(camera);
    glDepthFunc(GL_LEQUAL);
    ax_use_shader(&skybox.shader);

    // comeback and fix this
    glUniformMatrix4fv(glGetUniformLocation(skybox.shader.ID, "view"), 1, GL_FALSE, (float*)camera->view);
    glUniformMatrix4fv(glGetUniformLocation(skybox.shader.ID, "projection"), 1, GL_FALSE, (float*)camera->projection);

    ax_bind_vao(skybox.vao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.cubemap.id);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);
}
