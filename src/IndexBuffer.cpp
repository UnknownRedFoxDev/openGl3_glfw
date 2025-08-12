#include "IndexBuffer.h"
#include "Renderer.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count) : mCount(count) {
    ASSERT(sizeof(GLuint) == sizeof(unsigned int));

    GLCall(glGenBuffers(1, &mRendereID));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRendereID));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}
IndexBuffer::~IndexBuffer() {
    GLCall(glDeleteBuffers(1, &mRendereID));
}

void IndexBuffer::Bind() const {
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRendereID));
}
void IndexBuffer::Unbind() const {
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
