#include <string>
#include <unordered_map>
#include "vendor/glm/glm.hpp"

#ifndef SHADER_H
#define SHADER_H

class Shader {
    private:
        std::string mVertShaderPath;
        std::string mFragShaderPath;
        unsigned int mRendererID;
        std::unordered_map<std::string, int> mUniformLocationCache;

        int GetUniformLocation(const std::string& name);
        std::string loadFile(std::string const& filepath);
        unsigned int CompileShader(unsigned int type, const std::string& source);
        unsigned int CreateProgram(const std::string& vertShaderPath, const std::string& fragShaderPath);

    public:
        Shader(const std::string& vertShaderPath, const std::string& fragShaderPath);
        ~Shader();

        void Bind() const;
        void Unbind() const;

        void SetUniform1i(const std::string& name, int value);
        void SetUniform1iv(const std::string& name, unsigned int count, int* value);
        void SetUniform1f(const std::string& name, float value);
        void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
        void SetUniformMat4f(const std::string& name, glm::mat4& matrix);
};

#endif
