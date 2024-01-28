#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../../lib/stb_image.h"

// TODO: clean this shit up

Texture texture_load(const char* filename)
{
    Texture texture;
    memset(&texture, 0, sizeof(Texture));
    
    int width, height, num_channels;
    unsigned char* image_data = stbi_load(filename, &width, &height, &num_channels, 0);
    if (!image_data) {
        fprintf(stderr, "Failed to load texture: %s\n", filename);
        return texture;
    }

    glGenTextures(1, &texture.id);
    glBindTexture(GL_TEXTURE_2D, texture.id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GLenum format = (num_channels == 1) ? GL_RED : (num_channels == 3) ? GL_RGB : GL_RGBA;
    
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image_data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(image_data);
    return texture;
}

Texture load_model_texture(const char* filename, const char* type_name)
{
    Texture texture;
    memset(&texture, 0, sizeof(Texture));

    glGenTextures(1, &texture.id);

    int width, height, num_channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(filename, &width, &height, &num_channels, 0);
    if (!data) {
        fprintf(stderr, "Failed to load texture: %s\n", filename);
        stbi_image_free(data);
    }

    GLenum format = (num_channels == 1) ? GL_RED : (num_channels == 3) ? GL_RGB : GL_RGBA;

    glBindTexture(GL_TEXTURE_2D, texture.id);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
    
    strncpy(texture.type, type_name, sizeof(texture.type) - 1);
    texture.type[sizeof(texture.type) - 1] = '\0';
    strncpy(texture.path, filename, sizeof(texture.path) - 1);
    texture.path[sizeof(texture.path) - 1] = '\0';

    return texture;
}

Texture load_cubemap_texture(const char* faces[6])
{
    Texture texture;
    glGenTextures(1, &texture.id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture.id);

    for (int i = 0; i < 6; ++i) {
        stbi_set_flip_vertically_on_load(false);
        int width, height, num_channels;
        unsigned char* data = stbi_load(faces[i], &width, &height, &num_channels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);            
        } else {
            fprintf(stderr, "Failed to load texture: %s\n", faces[i]);
            return texture;
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return texture;
}

void bind_texture(Texture *texture)
{
    glBindTexture(GL_TEXTURE_2D, texture->id);
}

void unbind_texture(Texture *texture)
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void destroy_texture(Texture *texture)
{
    glDeleteTextures(1, &texture->id);
    texture->id = 0;

    memset(texture->type, 0, sizeof(texture->type));
    memset(texture->path, 0, sizeof(texture->path));
}

