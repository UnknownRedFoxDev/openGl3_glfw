#pragma once

// :s/^\([^aA-zZ]*\)\([^;]*\)/\1GLCall(\2) my beloved
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include <GL/glew.h>
#include <iostream>

#define ASSERT(x) if (!(x)) {\
    std::cout << __FILE__ << ":" << __LINE__ << " - \"" << #x << "\" failed assertion" << std::endl; \
    exit(-1); \
}
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))
#define DEBUG 1

typedef unsigned int uint;

void GLClearError();

// Returns `true` when no error is detected.
// Returns `false` otherwise.
bool GLLogCall(const char* funcName, const char* filename, int line);


class Renderer {
    public:
        void Draw(const VertexArray& vao, const IndexBuffer& ibo, const Shader& shaderProgram) const;
        void Clear() const;
};
