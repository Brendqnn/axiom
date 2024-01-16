#ifndef EBO_H
#define EBO_H

#include <GL/glew.h>

typedef struct {
    GLuint ID;
} EBO;

/**
 * Create an Element Buffer Object (EBO).
 *
 * @return An EBO structure representing the created EBO.
 */
EBO ebo_create();

/**
 * Destroy an Element Buffer Object (EBO).
 *
 * @param ebo The EBO structure to be destroyed.
 */
void ebo_destroy(EBO ebo);

/**
 * Bind the Element Buffer Object (EBO) for rendering.
 *
 * @param ebo The EBO structure to be bound.
 */
void ebo_bind(EBO ebo);

/**
 * Unbind the currently bound Element Buffer Object (EBO).
 */
void ebo_unbind();

/**
 * Buffer data into the Element Buffer Object (EBO).
 *
 * @param ebo     The EBO structure to which data will be buffered.
 * @param indices The array of indices to be buffered.
 * @param size    The size of the data to be buffered.
 */
void ebo_buffer(EBO ebo, unsigned int indices[], size_t size);

#endif // EBO_H
