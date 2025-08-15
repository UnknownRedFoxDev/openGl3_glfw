#pragma once

#include <string>
#include <vector>

class Texture {
    private:
        unsigned int m_Renderer;
        int m_Width, m_Height, m_BitPerPixel;
        std::string m_FilePath;
        unsigned char* m_DataBuffer;
        void BindToRenderer();

    public:
        Texture(const std::string& path);
        Texture(std::vector<unsigned char> data, int width, int height, int channels);
        Texture(unsigned char* data, int width, int height, int channels);
        ~Texture();

        void Bind(unsigned int slot = 0) const;
        void Unbind() const;

        inline int GetWidth() const { return m_Width; }
        inline int GetHeight() const { return m_Height; }
        inline int GetBPP() const { return m_BitPerPixel; }
        inline int GetTexID() const { return m_Renderer; }
};
