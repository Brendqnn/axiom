#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


typedef struct Texture {
	 GLuint id;
	GLenum target;
	int width;
	int height;
} Texture;

Texture texture_load(const char *filename, bool enable_asf);
void texture_bind(const Texture *texture, GLuint texture_unit);
void texture_unbind(const Texture *texture);
void texture_cleanup(Texture *texture);

#endif // TEXTURE_H
