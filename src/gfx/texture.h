#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>

typedef struct Texture {
	GLuint id;
	GLenum target;
} Texture;

Texture texture_load(const char *filename);

#endif // TEXTURE_H
