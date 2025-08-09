#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <alloca.h>

#include "Renderer.h"

#include "Test.h"
#include "TestTexture2D.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"

#include "TestClearColor.h"
#include "TestSplitSpriteSheet.h"

constexpr int WINDOWWIDTH = 960, WINDOWHEIGHT = 540;

int main(void) {
	if (!glfwInit()) {
#if DEBUG
		std::cerr << __FILE__ << ":" << __LINE__ <<  " - Unable to initialise glfw" << std::endl;
#endif
		return -1;
	}

	GLFWwindow* window;

	const char* glsl_version = "#version 330";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(WINDOWWIDTH, WINDOWHEIGHT, "Glfw learning with opengl <3", NULL, NULL);
	if (!window) {
#if DEBUG
		std::cerr << __FILE__ << ":" << __LINE__ <<  " - Unable to create window" << std::endl;
#endif
		glfwTerminate();
		return -2;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK) {
#if DEBUG
		std::cerr << __FILE__ << ":" << __LINE__ <<  " - Unable to initialize GLEW" << std::endl;
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
	testMenu->RegisterTest<test::TestTexture2D>("2D Texture");
	testMenu->RegisterTest<test::TestSplitSpriteSheet>("Sprite Sheet Split");

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
	else
		delete currentTest;

	ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
