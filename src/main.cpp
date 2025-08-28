#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include "Renderer.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"

#include "TestClearColor.h"
#include "TestTexture2D.h"
#include "TestSplitSpriteSheet.h"
#include "TestRefactoring.h"
#include "TestRefactoring2.h"
#include "TestBatchRenderingTextures.h"
#include "TestCardDecks.h"
#include "TestCardObj.h"
#include "TestPlayerDeck.h"

constexpr int WINDOW_WIDTH = 1920, WINDOW_HEIGHT = 1080;

int main(void) {
    if (!glfwInit()) {
#if DEBUG
        std::cerr << __FILE__ << ":" << __LINE__ <<  " - Unable to initialise glfw" << std::endl;
#endif
        return -1;
    }

    GLFWwindow* window;

    const char* glsl_version = "#version 450";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Glfw learning with opengl <3", NULL, NULL);
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
	testMenu->RegisterTest<test::TestRefactoring>("Refactoring");
	testMenu->RegisterTest<test::TestBatchRenderingTextures>("Batch Rendering Textures");
	testMenu->RegisterTest<test::TestRefactoring2>("2nd Refactoring");
	testMenu->RegisterTest<test::TestDecks>("Decks");
	testMenu->RegisterTest<test::TestCardObj>("Card object", WINDOW_WIDTH, WINDOW_HEIGHT);
	testMenu->RegisterTest<test::TestPlayerDeck>("Player Deck", WINDOW_WIDTH, WINDOW_HEIGHT);

    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);


    GLCall(glClearColor(0.086f, 0.086f, 0.086f, 0.0f));

    // Mainframe* drawPile = new Mainframe;
    // Player* first = new Player();
    // Player* second = new Player();

    // Main Loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        renderer.Clear();


        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (currentTest) {
			currentTest->OnUpdate();
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
    }
    if (currentTest == testMenu) {
		delete testMenu;
    } else {
		delete currentTest;
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
