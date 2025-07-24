#pragma once

 #include <string>
class Texture {
	 private:
		 unsigned int m_Renderer;
		 int m_Width, m_Height;
		 std::string m_FilePath;
		 unsigned char* m_DataBuffer;
	public:
		 Texture(const std::string& path);
		 ~Texture();

		 void Bind(unsigned int slot = 0) const;
		 void Unbind() const;
};
