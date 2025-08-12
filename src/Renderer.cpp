#include "Renderer.h"
#include <iostream>

void GLClearError() {
    while (glGetError() != GL_NO_ERROR);
}

// Returns `true` when no error is detected.
// Returns `false` otherwise.
bool GLLogCall(const char* funcName, const char* filename, int line) {
    while (GLenum error = glGetError()) {
        std::cout << filename << ":" << line << " - OpenGL ERROR (" << error << "): from " << funcName << std::endl;
        return false;
    }
    return true;
}


void Renderer::Draw(const VertexArray& vao, const IndexBuffer& ebo, const Shader& shaderProgram) const {
    shaderProgram.Bind();
    vao.Bind();
    ebo.Bind(); // Element buffer object rather than index buffer object
    GLCall(glDrawElements(GL_TRIANGLES, ebo.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::Clear() const {
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}
