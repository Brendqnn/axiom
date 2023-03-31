#ifndef SHADER_H
#define SHADER_H

#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

GLuint shader_load(const char* vertex_shader, const char* frag_shader);


#endif
