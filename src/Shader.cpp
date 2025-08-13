#include "Shader.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include "Renderer.h"

std::string Shader::loadFile(std::string const& filepath) {
    std::ifstream stream(filepath);
    std::stringstream ss;
    ss << stream.rdbuf();
    return ss.str();
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
    GLCall(unsigned int shaderID = glCreateShader(type));
    size_t length = source.length();
    if (!length) {
        fprintf(stderr, "%s:%d - Path provided is empty or out of scope\n", __FILE__, __LINE__);
        return 0;
    }
    const char* src = source.c_str();
    GLCall(glShaderSource(shaderID, 1, &src, NULL)); // nullptr defines src as a null-terminated array of character. Making the computation automatic rather than inputting a defined length.
    GLCall(glCompileShader(shaderID));

    // Error Handling
    int compiledCorrectly = 0;
    GLCall(glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compiledCorrectly));
    if (!compiledCorrectly) {
        int length;
        GLCall(glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length));

        char* log = (char*)alloca(length * sizeof(char));
        GLCall(glGetShaderInfoLog(shaderID, length, &length, log));
        // Currently only working with vertex and fragment shader.
        fprintf(stderr, "%s:%d - %s shader failed compilation.\n", __FILE__, __LINE__, type == GL_VERTEX_SHADER ? "Vertex" : "Fragment");
        fprintf(stderr, "%s:%d - Log from shader infos: %s\n", __FILE__, __LINE__, log);
        GLCall(glDeleteShader(shaderID));
        return 0;
    }

    return shaderID;
}

unsigned int Shader::CreateProgram(const std::string& vertShaderPath, const std::string& fragShaderPath) {
    GLCall(unsigned int programID = glCreateProgram());
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, loadFile(vertShaderPath));
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, loadFile(fragShaderPath));
    GLCall(glAttachShader(programID, vs)); // Merge vertex shader with the program
    GLCall(glAttachShader(programID, fs)); // Merge fragment shader with the program
    GLCall(glLinkProgram(programID)); // Links the program to the GPU for usage. -> will replace any default program !
    GLCall(glValidateProgram(programID));

    // Error Handling
    int isValid = 0;
    GLCall(glGetProgramiv(programID, GL_VALIDATE_STATUS, &isValid));
    if (!isValid) {
        int length;
        GLCall(glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &length));

        char* log = (char*)alloca(length * sizeof(char));
        GLCall(glGetProgramInfoLog(programID, length, &length, log));
        fprintf(stderr, "%s:%d - Program failed validation. See above for compile shader errors.\n", __FILE__, __LINE__);
        fprintf(stderr, "%s:%d - Log from program infos: %s\n", __FILE__, __LINE__, log);
        GLCall(glDeleteProgram(programID));
        return 0;
    }

    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    return programID;
}


Shader::Shader(const std::string& vertShaderPath, const std::string& fragShaderPath)
    : mVertShaderPath(vertShaderPath), mFragShaderPath(fragShaderPath), mRendererID(0) {
        mRendererID = CreateProgram(vertShaderPath, fragShaderPath);
    }

Shader::~Shader() {
    GLCall(glDeleteProgram(mRendererID));
}

void Shader::Bind() const {
    GLCall(glUseProgram(mRendererID));
}

void Shader::Unbind() const {
    GLCall(glUseProgram(0));
}

int Shader::GetUniformLocation(const std::string& name) {
    if (mUniformLocationCache.find(name) != mUniformLocationCache.end()) {
        return mUniformLocationCache[name];
    }

    GLCall(int location = glGetUniformLocation(mRendererID, name.c_str()));
    if (location == -1) std::cout << "Uniform \"" << name << "\" was not found" << std::endl;
    mUniformLocationCache[name] = location;
    return location;
}

void Shader::SetUniform1i(const std::string& name, int value) {
    GLCall(glUniform1i(GetUniformLocation(name), value));
}
void Shader::SetUniform1iv(const std::string& name, unsigned int count, int* value) {
    GLCall(glUniform1iv(GetUniformLocation(name), count, value));
}

void Shader::SetUniform1f(const std::string& name, float value) {
    GLCall(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) {
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniformMat4f(const std::string& name, glm::mat4& matrix) {
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}
