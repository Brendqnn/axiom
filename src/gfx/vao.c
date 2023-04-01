#include "vao.h"


void setup_vao(GLuint vao) {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
}
