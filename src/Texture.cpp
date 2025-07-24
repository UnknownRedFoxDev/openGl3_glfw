#include "Texture.h"

Texture::Texture(const std::string& path) : m_Renderer(0), m_FilePath(path), m_DataBuffer(0), m_Height(0), m_Width(0) {
}

Texture::~Texture(){
}

void Texture::Bind(unsigned int slot) const {
}

void Texture::Unbind() const {
}
