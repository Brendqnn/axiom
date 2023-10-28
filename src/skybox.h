#ifndef SKYBOX_H
#define SKYBOX_H

#include <GL/glew.h>
#include "camera.h"
#include "shader.h"

typedef struct Skybox {
    unsigned int cube_map;
    unsigned int vao;
    unsigned int vbo;
    Shader sb_shader;
    int view_loc
    int projection_loc;
} Skybox;

void skybox_init(Skybox* skybox, const char* skybox_file_path);
void skybox_render(Skybox* skybox, Camera* camera);
void skybox_cleanup(Skybox* skybox);

#endif  // SKYBOX_H
