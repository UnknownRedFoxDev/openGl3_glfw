#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <alloca.h>
#include <cstdio>
#include <iostream>
#include <string>
#include <sys/types.h>
#define DEBUG 1

static uint CompileShader(uint type, const std::string& source) {
	uint shaderID = glCreateShader(type);
	size_t length = source.length();
	if (!length) {
#if DEBUG
		fprintf(stderr, "%s:%d - Path provided is empty or out of scope\n", __FILE__, __LINE__);
#endif
		return 0;
	}
	const char* src = source.c_str();
	glShaderSource(shaderID, 1, &src, NULL); // nullptr defines src as a null-terminated array of character. Making the computation automatic rather than inputting a defined length.
	glCompileShader(shaderID);

	// Error Handling
	int compiledCorrectly = 0;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compiledCorrectly);
    if (!compiledCorrectly) {
		int length;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length);

		char* log = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(shaderID, length, &length, log);
#if DEBUG
		// Currently only working with vertex and fragment shader.
		fprintf(stderr, "%s:%d - %s shader failed compilation.\n", __FILE__, __LINE__, type == GL_VERTEX_SHADER ? "Vertex" : "Fragment");
		fprintf(stderr, "%s:%d - Log from shader infos: %s\n", __FILE__, __LINE__, log);
#endif
		glDeleteShader(shaderID);
		return 0;
    }

	return shaderID;
}

static uint CreateShader(const std::string& vertShaderPath, const std::string& fragShaderPath) {
	uint programID = glCreateProgram();
	uint vs = CompileShader(GL_VERTEX_SHADER, vertShaderPath);
	uint fs = CompileShader(GL_FRAGMENT_SHADER, vertShaderPath);
	glAttachShader(programID, vs); // Merge vertex shader with the program
	glAttachShader(programID, fs); // Merge fragment shader with the program
	glLinkProgram(programID); // Links the program to the GPU for usage. -> will replace any default program !
	glValidateProgram(programID);

	// Error Handling
	int isValid = 0;
	glGetProgramiv(programID, GL_VALIDATE_STATUS, &isValid);
	if (!isValid) {
		int length;

		char log[512];
		glGetProgramInfoLog(programID, 512, nullptr, log);
#if DEBUG
		fprintf(stderr, "%s:%d - Program failed validation. See above for compile shader errors.\n", __FILE__, __LINE__);
		fprintf(stderr, "%s:%d - Log from program infos: %s\n", __FILE__, __LINE__, log);
#endif
		glDeleteProgram(programID);
		return 0;
	}

	glDeleteShader(vs);
	glDeleteShader(fs);

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

	window = glfwCreateWindow(800, 600, "Glfw learning with opengl <3", NULL, NULL);
	if (!window) {
#if DEBUG
		fprintf(stderr, "%s:%i - Unable to create window\n", __FILE__, __LINE__);
#endif
		glfwTerminate();
		return -2;
	}

	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK) {
#if DEBUG
    fprintf(stderr, "%s:%i - Unable to initialize GLEW\n", __FILE__, __LINE__);
#endif
    return -3;
	}

	float vertices[6] = {
		-0.5f, -0.5f,
		 0.0f, 0.5f,
		 0.5f, -0.5f
	};

	GLuint vertexBufferID;
	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), vertices, GL_STATIC_DRAW);

	while (!glfwWindowShouldClose(window)) {
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
