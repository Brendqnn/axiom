#include "vbo.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>


void setup_vbo(GLuint vbo) {
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5, 0.0f,
		0.0f, 0.5f, 0.0f
	};

   
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    
}
