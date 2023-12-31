#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>


typedef struct {
    unsigned int id;
    char type[32];
    char path[128];    
} Texture;

Texture load_texture(const char* filename);
Texture load_model_texture(const char* filename, const char* type_name);
Texture load_cubemap_texture(const char* faces[6]);
void bind_texture(Texture *texture);
void unbind_texture(Texture *texture);
void destroy_texture(Texture *texture);

#endif // TEXTURE_H
