// :s/^\([^aA-zZ]*\)\([^;]*\)/\1GLCall(\2) my beloved
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <alloca.h>
#include <cassert>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#define ASSERT(x) if (!(x)) exit(-1);
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))
#define DEBUG 1

typedef unsigned int uint;

static void GLClearError() {
	while (glGetError() != GL_NO_ERROR);
}

// Returns `true` when no error is detected.
// Returns `false` otherwise.
static bool GLLogCall(const char* funcName, const char* filename, int line) {
	while (GLenum error = glGetError()) {
		std::cout << filename << ":" << line << " - OpenGL ERROR (" << error << "): from " << funcName << std::endl;
		return false;
	}
	return true;
}

static std::string loadFile(std::string const& filepath) {
	std::ifstream stream(filepath);
	std::stringstream ss;
	ss << stream.rdbuf();
	return ss.str();
}

static uint CompileShader(uint type, const std::string& source) {
	GLCall(uint shaderID = glCreateShader(type));
	size_t length = source.length();
	if (!length) {
		fprintf(stderr, "%s:%d - Path provided is empty or out of scope\n", __FILE__, __LINE__);
		return 0;
	}
	const char* src = source.c_str();
	GLCall(glShaderSource(shaderID, 1, &src, NULL)); // nullptr defines src as a null-terminated array of character. Making the computation automatic rather than inputting a defined length.
	glCompileShader(shaderID);

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

static uint CreateProgram(const std::string& vertShaderPath, const std::string& fragShaderPath) {
	GLCall(uint programID = glCreateProgram());
	uint vs = CompileShader(GL_VERTEX_SHADER, loadFile(vertShaderPath));
	uint fs = CompileShader(GL_FRAGMENT_SHADER, loadFile(fragShaderPath));
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

int main(void) {
	GLFWwindow* window;
	if (!glfwInit()) {
#if DEBUG
		fprintf(stderr, "%s:%i - Unable to initialise glfw\n", __FILE__, __LINE__);
#endif
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(800, 600, "Glfw learning with opengl <3", NULL, NULL);
	if (!window) {
#if DEBUG
		fprintf(stderr, "%s:%i - Unable to create window\n", __FILE__, __LINE__);
#endif
		glfwTerminate();
		return -2;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK) {
#if DEBUG
    fprintf(stderr, "%s:%i - Unable to initialize GLEW\n", __FILE__, __LINE__);
#endif
    return -3;
	}

	float vertices[] = {
		-0.5f, -0.5f,
		 0.5f, -0.5f,
		 0.5f,  0.5f,
		-0.5f,  0.5f,
	};

	uint indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	uint vao;
	GLCall(glGenVertexArrays(1, &vao));
	GLCall(glBindVertexArray(vao));

	// Binding the vertex buffer
	uint vertexBufferID;
	GLCall(glGenBuffers(1, &vertexBufferID));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID));
	GLCall(glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), vertices, GL_STATIC_DRAW));

	// Setting the attributs for the vertex buffer
	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0));

	// Binding the element buffer object (serves as the indices for each vertex in the vertex buffer)
	uint ebo;
	GLCall(glGenBuffers(1, &ebo));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW));

	// Create a shader program and use it
	uint programID = CreateProgram("res/shaders/shader.vert", "res/shaders/shader.frag");
	GLCall(glUseProgram(programID));

	GLCall(int location = glGetUniformLocation(programID, "uColour"));
	assert(location != -1);
	GLCall(glUniform4f(location, 0.45, 0.55, 0.60, 1.00));

	float red = 0.45f;
	float val = 0.05f;
	while (!glfwWindowShouldClose(window)) {
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		GLCall(glUniform4f(location, red, 0.55, 0.60, 1.00));
		GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));

		if (red > 1) val = -0.05f;
		else if (red < 0) val = 0.05f;

		red += val;

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	GLCall(glDeleteProgram(programID));
	glfwTerminate();
	return 0;
}
