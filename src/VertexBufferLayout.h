#ifndef VERTEX_BUFFER_LAYOUT_H
#define VERTEX_BUFFER_LAYOUT_H

#include <vector>
#include <GL/glew.h> // For OpenGL types

struct VertexBufferElement {
    GLenum type;
    unsigned int count;
    GLboolean normalized;

    static unsigned int GetTypeSize(GLenum type) {
        switch (type) {
            case GL_FLOAT:         return sizeof(GLfloat);
            case GL_UNSIGNED_INT:  return sizeof(GLuint);
            case GL_UNSIGNED_BYTE: return sizeof(GLubyte);
        }
        return 0;
    }
};

class VertexBufferLayout {
    private:
        std::vector<VertexBufferElement> mElements;
        unsigned int mStride;

    public:
        VertexBufferLayout() : mStride(0) {}

        template<typename T>
            void Push(unsigned int count);

        inline const std::vector<VertexBufferElement>& GetElements() const { return mElements; }
        inline unsigned int GetStride() const { return mStride; }
};

#endif
