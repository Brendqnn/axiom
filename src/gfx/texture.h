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
    unsigned int count;
} Texture;

/**
 * Load a texture from a file.
 *
 * @param filename The path to the texture file.
 * @return         A Texture structure representing the loaded texture.
 */
Texture load_texture(const char* filename);

/**
 * Load a texture for a model from a file with a specified type.
 *
 * @param filename   The path to the texture file.
 * @param type_name  The type of the texture.
 * @return           A Texture structure representing the loaded texture for the model.
 */
Texture load_model_texture(const char* filename, const char* type_name);

/**
 * Find a loaded texture from an array based on its path.
 *
 * @param textures       The array of loaded textures.
 * @param num_textures   The number of loaded textures.
 * @param path           The path of the texture to find.
 * @return               A Texture structure representing the found texture.
 */
Texture find_loaded_texture(Texture* textures, unsigned int num_textures, const char* path);

/**
 * Load a cubemap texture from an array of file paths.
 *
 * @param faces An array of file paths for the six faces of the cubemap.
 * @return      A Texture structure representing the loaded cubemap texture.
 */
Texture load_cubemap_texture(const char* faces[6]);

/**
 * Bind the texture for rendering.
 *
 * @param texture The Texture structure to be bound.
 */
void bind_texture(Texture *texture);

/**
 * Unbind the currently bound texture.
 *
 * @param texture The Texture structure to be unbound.
 */
void unbind_texture(Texture *texture);

/**
 * Destroy a texture.
 *
 * @param texture The Texture structure to be destroyed.
 */
void destroy_texture(Texture *texture);

#endif // TEXTURE_H
