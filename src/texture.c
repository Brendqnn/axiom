#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "gfx/stb_image.h"


Texture texture_load(const char* filename, bool enable_asf) {
    Texture texture;
    memset(&texture, 0, sizeof(Texture));

    // Load image using stb_image library
    int width, height, numChannels;
    unsigned char* imageData = stbi_load(filename, &width, &height, &numChannels, 0);
    if (!imageData) {
        fprintf(stderr, "Failed to load texture: %s\n", filename);
        return texture;
    }

    // Generate and bind texture object
    glGenTextures(1, &texture.id);
    glBindTexture(GL_TEXTURE_2D, texture.id);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Enable anisotropic filtering if requested
    if (enable_asf && glewIsExtensionSupported("GL_EXT_texture_filter_anisotropic")) {
        float maxAnisotropy;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);
    }

    // Determine the texture format based on the number of channels
    GLenum format;
    if (numChannels == 1)
        format = GL_RED;
    else if (numChannels == 3)
        format = GL_RGB;
    else if (numChannels == 4)
        format = GL_RGBA;
    else {
        fprintf(stderr, "Invalid number of channels: %d\n", numChannels);
        stbi_image_free(imageData);
        return texture;
    }

    // Upload texture data
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, imageData);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);

    // Free the image data
    stbi_image_free(imageData);

    // Set the texture target
    texture.target = GL_TEXTURE_2D;

    return texture;
}

void texture_bind(const Texture* texture, GLuint textureUnit) {
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(texture->target, texture->id);
}

void texture_unbind(const Texture* texture) {
    glBindTexture(texture->target, 0);
}

void texture_cleanup(Texture* texture) {
    glDeleteTextures(1, &texture->id);
    texture->id = 0;
    texture->target = 0;

    // Reset texture parameters
    glTexParameteri(texture->target, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(texture->target, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(texture->target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(texture->target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}


