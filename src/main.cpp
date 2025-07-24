#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <alloca.h>
#include <iostream>

#include "Renderer.h"

#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"
#include "Shader.h"

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

	window = glfwCreateWindow(1920, 1080, "Glfw learning with opengl <3", NULL, NULL);
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
		-0.5f, -0.5f, 0.0f, 0.0f,
		 0.5f, -0.5f, 1.0f, 0.0f,
		 0.5f,  0.5f, 1.0f, 1.0f,
		-0.5f,  0.5f, 0.0f, 1.0f,
	};

	uint indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	// Creating the vertex array, buffer and layout
	VertexArray va;
	VertexBuffer vb(vertices, 4 * 4 * sizeof(float));
	VertexBufferLayout layout;

	// Create a layout with two elements
	layout.Push<float>(2);
	layout.Push<float>(2);

	// Add that layout and buffer to the vertex array
	va.AddBuffer(vb, layout);

	// Binding the element buffer object (serves as the indices for each vertex in the vertex buffer)
	IndexBuffer eb(indices, 6);

	// Create a shader program and use it
	Shader shaderProgram("res/shaders/shader.vert", "res/shaders/shader.frag");
	shaderProgram.Bind();
	std::string colourUniform = "uColour";
	// shaderProgram.SetUniform4f(colourUniform, 0.45, 0.55, 0.60, 1.00);

	Renderer renderer;

	va.Unbind();
	vb.Unbind();
	eb.Unbind();
	shaderProgram.Unbind();

	Texture texture("res/textures/test.png");
	texture.Bind();
	shaderProgram.Bind();
	std::string textureUniform = "uTexture";
	shaderProgram.SetUniform1i(textureUniform, 0);

	float red = 0.45f;
	float val = 0.05f;
	while (!glfwWindowShouldClose(window)) {
		renderer.Clear();

		shaderProgram.Bind();
		shaderProgram.SetUniform1i(textureUniform, 0);
		// shaderProgram.SetUniform4f(colourUniform, red, 0.55, 0.60, 1.00);

		renderer.Draw(va, eb, shaderProgram);

		if (red > 1) val = -0.05f;
		else if (red < 0) val = 0.05f;

		red += val;

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
