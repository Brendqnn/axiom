#ifndef AXTEXTURE_H
#define AXTEXTURE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct {
    unsigned int id;
    char type[32];
    char path[128];

    unsigned int count;
} AXTexture;

AXTexture ax_load_texture(const char* filename);
AXTexture ax_load_model_texture(const char* filename, const char* type_name);
AXTexture ax_find_loaded_texture(AXTexture* textures, unsigned int num_textures, const char* path);
AXTexture ax_load_cubemap_textures(const char* faces[6]);
void ax_bind_texture(AXTexture *texture);
AXTexture ax_load_cubemap_from_file(const char *filename);
void ax_free_texture(AXTexture *texture);

#endif // AXTEXTURE_H
