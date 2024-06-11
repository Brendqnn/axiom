
#include "axtexture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../../lib/stb_image.h"


AXTexture ax_load_texture(const char* filename)
{
    AXTexture texture;
    memset(&texture, 0, sizeof(AXTexture));

    glGenTextures(1, &texture.id);
    glBindTexture(GL_TEXTURE_2D, texture.id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    int width, height, num_channels;
    unsigned char* image_data = stbi_load(filename, &width, &height, &num_channels, 0);
    if (!image_data) {
        fprintf(stderr, "Failed to load texture: %s\n", filename);
        return texture;
    }

    GLenum format = (num_channels == 1) ? GL_RED : (num_channels == 3) ? GL_RGB : GL_RGBA;

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image_data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(image_data);
    return texture;
}

AXTexture ax_load_model_texture(const char* filename, const char* type_name)
{
    AXTexture texture;
    memset(&texture, 0, sizeof(AXTexture));

    glGenTextures(1, &texture.id);
    glBindTexture(GL_TEXTURE_2D, texture.id);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, num_channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(filename, &width, &height, &num_channels, 0);
    if (!data) {
        fprintf(stderr, "Failed to load texture: %s\n", filename);
        stbi_image_free(data);
    }

    GLenum format = (num_channels == 1) ? GL_RED : (num_channels == 3) ? GL_RGB : GL_RGBA;

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
    
    strncpy(texture.type, type_name, sizeof(texture.type) - 1);
    texture.type[sizeof(texture.type) - 1] = '\0';
    strncpy(texture.path, filename, sizeof(texture.path) - 1);
    texture.path[sizeof(texture.path) - 1] = '\0';

    return texture;
}

AXTexture ax_load_cubemap_textures(const char* faces[6])
{
    AXTexture texture;

    glGenTextures(1, &texture.id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture.id);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

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

    return texture;
}

// TODO: figure out how to load skybox from a single file???
AXTexture ax_load_cubemap_from_file(const char *filename)
{
    AXTexture texture;
    glGenTextures(1, &texture.id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture.id);
    
    int width, height, num_channels;
    stbi_set_flip_vertically_on_load(false);
    unsigned char* data = stbi_load(filename, &width, &height, &num_channels, 0);
    if (!data) {
        fprintf(stderr, "Failed to load texture: %s\n", filename);
        stbi_image_free(data);
    }
    
    int face_width = width/4;
    int face_height = height/3;
    int face_channels = num_channels;

    for (int i = 0; i < 6; ++i) {
        int xoff = (i % 4) * face_width;
        int yoff = (i / 4) * face_height;

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, face_width, face_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data + (yoff * width + xoff) * face_channels); 
    }

    stbi_image_free(data);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return texture;
}

void ax_bind_texture(AXTexture *texture)
{
    glBindTexture(GL_TEXTURE_2D, texture->id);
}

void ax_destroy_texture(AXTexture *texture)
{
    glDeleteTextures(1, &texture->id);
    texture->id = 0;

    memset(texture->type, 0, sizeof(texture->type));
    memset(texture->path, 0, sizeof(texture->path));
}

