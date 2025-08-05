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

#include "tests/Test.h"
#include "vendor/glm/ext/matrix_transform.hpp"
#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"

#include "vendor/ImGui/imgui.h"
#include "vendor/ImGui/imgui_impl_glfw.h"
#include "vendor/ImGui/imgui_impl_opengl3.h"

#include "tests/TestClearColor.h"

int main(void) {
	if (!glfwInit()) {
#if DEBUG
		fprintf(stderr, "%s:%i - Unable to initialise glfw\n", __FILE__, __LINE__);
#endif
		return -1;
	}

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

	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	Renderer renderer;

	test::Test* currentTest = nullptr;
	test::TestMenu* testMenu = new test::TestMenu(currentTest);
	currentTest = testMenu; // Starts off with the menu

	testMenu->RegisterTest<test::TestClearColor>("Clear Color");

	ImGui::CreateContext();
    ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	// Main Loop
	while (!glfwWindowShouldClose(window)) {
		renderer.Clear();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		if (currentTest) {
			currentTest->OnUpdate(0.0f);
			currentTest->OnRender();

			ImGui::Begin("Test");
			if (currentTest != testMenu && ImGui::Button("<--")) {
				delete currentTest;
				currentTest = testMenu;
			}
			currentTest->OnImGuiRender();
			ImGui::End();
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	if (currentTest == testMenu)
		delete testMenu;
	delete currentTest;

	ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
