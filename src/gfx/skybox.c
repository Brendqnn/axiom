#include "skybox.h"


Skybox skybox_init(const char *skybox_faces[])
{
    Skybox skybox;

    float vertices[] = {
        -1.0f, -1.0f,  1.0f,//       7--------6
        1.0f, -1.0f,  1.0f,//       /|       /|
        1.0f, -1.0f, -1.0f,//      4--------5 |
        -1.0f, -1.0f, -1.0f,//     | |      | |
        -1.0f,  1.0f,  1.0f,//     | 3------|-2
        1.0f,  1.0f,  1.0f,//      |/       |/
        1.0f,  1.0f, -1.0f,//      0--------1
        -1.0f,  1.0f, -1.0f
    };

    unsigned int indices[] = {
        // Right
        1, 2, 6,
        6, 5, 1,
        // Left
        0, 4, 7,
        7, 3, 0,
        // Top
        4, 5, 6,
        6, 7, 4,
        // Bottom
        0, 3, 2,
        2, 1, 0,
        // Back
        0, 1, 5,
        5, 4, 0,
        // Front
        3, 7, 6,
        6, 2, 3
    };

    skybox.vao = vao_create();
    skybox.vbo = vbo_create(GL_ARRAY_BUFFER, false);
    skybox.ebo = ebo_create();
    
    vao_bind(skybox.vao);
    vbo_buffer(skybox.vbo, vertices, sizeof(vertices));
    ebo_buffer(skybox.ebo, indices, sizeof(indices));
    link_attrib(skybox.vao, skybox.vbo, 0, 3, GL_FLOAT, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    skybox.cubemap = load_cubemap_texture(skybox_faces);

    skybox.shader = shader_create("res/shaders/skybox.vert", "res/shaders/skybox.frag");
    
    return skybox;
}

void skybox_render(Skybox skybox, Shader shader, Camera *camera)
{
    remove_translation_matrix(camera);
    glDepthFunc(GL_LEQUAL);
    shader_use(&skybox.shader);
    glUniformMatrix4fv(glGetUniformLocation(skybox.shader.ID, "view"), 1, GL_FALSE, (float*)camera->view);
    glUniformMatrix4fv(glGetUniformLocation(skybox.shader.ID, "projection"), 1, GL_FALSE, (float*)camera->projection);

    vao_bind(skybox.vao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.cubemap.id);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);
}
