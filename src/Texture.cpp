#include "Texture.h"
#include "Renderer.h"
#include "vendor/stb_image/stb_image.h"
#include <iostream>

void Texture::BindToRenderer() {
    GLCall(glGenTextures(1, &m_Renderer));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_Renderer));

    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
}
Texture::Texture(const std::string& path) : m_Renderer(0), m_Width(0), m_Height(0), m_BitPerPixel(0), m_FilePath(path), m_DataBuffer(0)  {
    stbi_set_flip_vertically_on_load(1);

    m_DataBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BitPerPixel, 4);
    BindToRenderer();

    if (m_DataBuffer) {
        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_DataBuffer));
        GLCall(glBindTexture(GL_TEXTURE_2D, 0));
        stbi_image_free(m_DataBuffer);
    } else {
        std::cout << "\n" << __FILE__ << ":" << __LINE__ << " - Error: Failed to load texture" << std::endl;
        std::cout << stbi_failure_reason() << std::endl;
        exit(1);
    }
}

Texture::Texture(std::vector<unsigned char> data, int width, int height, int channels) {
    stbi_set_flip_vertically_on_load(1);
    m_Width = width;
    m_Height = height;
    m_BitPerPixel = channels;
    m_DataBuffer = data.data();
    BindToRenderer();

    if (m_DataBuffer) {
        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_DataBuffer));
        GLCall(glBindTexture(GL_TEXTURE_2D, 0));
    }
}
Texture::Texture(unsigned char* data, int width, int height, int channels) {
    stbi_set_flip_vertically_on_load(1);
    m_Width = width;
    m_Height = height;
    m_BitPerPixel = channels;
    m_DataBuffer = data;
    BindToRenderer();

    if (m_DataBuffer) {
        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_DataBuffer));
        GLCall(glBindTexture(GL_TEXTURE_2D, 0));
    }
}

Texture::~Texture(){
    GLCall(glDeleteTextures(1, &m_Renderer));
}

void Texture::Bind(unsigned int slot) const {
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_Renderer));
}

void Texture::Unbind() const {
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
