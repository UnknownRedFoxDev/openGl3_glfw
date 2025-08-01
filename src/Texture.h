#pragma once

#include <string>

class Texture {
	 private:
		 unsigned int m_Renderer;
		 int m_Width, m_Height, m_BitPerPixel;
		 std::string m_FilePath;
		 unsigned char* m_DataBuffer;
	public:
		 Texture(const std::string& path);
		 ~Texture();

		 void Bind(unsigned int slot = 0) const;
		 void Unbind() const;

		 inline int GetWidth() const { return m_Width; }
		 inline int GetHeight() const { return m_Height; }
};
