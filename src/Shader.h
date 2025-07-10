#include <string>

#ifndef SHADER_H
#define SHADER_H

class Shader {
	private:
		std::string mVertShaderPath;
		std::string mFragShaderPath;

		unsigned int mRendererID;
		unsigned int GetUniformLocation(const std::string& name);
		std::string loadFile(std::string const& filepath);
		uint CompileShader(uint type, const std::string& source);
		uint CreateProgram(const std::string& vertShaderPath, const std::string& fragShaderPath);

	public:
		Shader(const std::string& vertShaderPath, const std::string& fragShaderPath);
		~Shader();

		void Bind() const;
		void Unbind() const;

		void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
};

#endif
