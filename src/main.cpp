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

#include "vendor/glm/ext/matrix_transform.hpp"
#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"

#include "vendor/ImGui/imgui.h"
#include "vendor/ImGui/imgui_impl_glfw.h"
#include "vendor/ImGui/imgui_impl_opengl3.h"

int main(void) {
	if (!glfwInit()) {
#if DEBUG
		fprintf(stderr, "%s:%i - Unable to initialise glfw\n", __FILE__, __LINE__);
#endif
		return -1;
	}

	float main_scale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor());
	GLFWwindow* window;

	constexpr int windowWidth = 960, windowHeight = 540;
	const char* glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(windowWidth, windowHeight, "Glfw learning with opengl <3", NULL, NULL);
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
		0.0f, 0.0f, 0.0f, 0.0f,
		100.0f, 0.0f, 1.0f, 0.0f,
		100.0f, 100.0f, 1.0f, 1.0f,
		0.0f, 100.0f, 0.0f, 1.0f,
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

	// ortho-cartisian plane so you multiply the bounds by 2 to get the ascept ratio (e.g. 2.0f x2 = 4 : 1.5f x 2 = 3, thus 4:3)
	// so for 16:9 it's -8.0f, 8.0f, -4.5f, 4.5f, -1.0f, 1.0f
	// I'd assume that it's -x, x, -y, y, -z, z
	glm::mat4 projectionMatrix = glm::ortho(0.0f, (float)(windowWidth), 0.0f, (float)(windowHeight), -1.0f, 1.0f);

	glm::vec3 camera_pos(200, 200, 0);
	glm::vec3 model_pos(0, 0, 0);

	// Create a shader program and use it
	Shader shaderProgram("res/shaders/shader.vert", "res/shaders/shader.frag");
	shaderProgram.Bind();
	// shaderProgram.SetUniform4f("u_Colour", 0.8f, 0.3f, 0.8f, 1.0f);

	Texture texture("res/textures/test.png");
	texture.Bind();
	std::string textureUniform = "uTexture";
	shaderProgram.SetUniform1i(textureUniform, 0);

	Renderer renderer;

	va.Unbind();
	vb.Unbind();
	eb.Unbind();
	shaderProgram.Unbind();

	// ImGui Stuff Here
	ImGui::CreateContext();
	// ImGuiIO& io = ImGui::GetIO(); (void)io;
	// io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup scaling
    ImGuiStyle& style = ImGui::GetStyle();
	style.ScaleAllSizes(main_scale);
	style.FontScaleDpi = main_scale;

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	// Main Loop
	while (!glfwWindowShouldClose(window)) {
		/* Poll for and process events */
		glfwPollEvents();
		if (glfwGetWindowAttrib(window, GLFW_ICONIFIED)) {
			ImGui_ImplGlfw_Sleep(10);
			continue;
		}
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		{
			ImGui::Begin("Debug");
			ImGui::DragFloat3("Camera position", &camera_pos.x, 1.0f, 0.0f, (float)(windowWidth), "%.2f");
			ImGui::DragFloat3("Model position", &model_pos.x, 1.0f, 0.0f, (float)(windowWidth), "%.2f");
			ImGui::End();
		}

		renderer.Clear();

		glm::mat4 view = glm::translate(glm::mat4(1.0f), camera_pos);
		glm::mat4 model = glm::translate(glm::mat4(1.0f), model_pos);
		glm::mat4 mvp = projectionMatrix * view * model;

		shaderProgram.Bind();
		shaderProgram.SetUniformMat4f("u_MVP", mvp);

		renderer.Draw(va, eb, shaderProgram);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		/* Swap front and back buffers */
		glfwSwapBuffers(window);
	}
	ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
