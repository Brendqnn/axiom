#include "shader.h"
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>



GLuint shader_load(const char* vertex_shader, const char* frag_shader) {
	GLuint v_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(v_shader, 1, &vertex_shader, NULL);
	glCompileShader(v_shader);

	GLuint f_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(f_shader, 1, &frag_shader, NULL);
	glCompileShader(f_shader);

	// create shader program
	GLuint shader_program = glCreateProgram();
	glAttachShader(shader_program, v_shader);
	glAttachShader(shader_program, f_shader);
	glLinkProgram(shader_program);

	
	glDeleteShader(v_shader);
	glDeleteShader(f_shader);

	return shader_program;
}

