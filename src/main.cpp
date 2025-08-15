#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <string>
#include <utility>

#include "Renderer.h"
#include "Pipeline.h"
#include "ImageManipulation.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"

constexpr int WINDOWWIDTH = 1920, WINDOWHEIGHT = 1080;
#define SUB_SPRITE_WIDTH 64
#define SUB_SPRITE_HEIGHT 94

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
    int cardTexIndex1 = 0, cardTexIndex2 = 0;
    float textureScale = 2.2f;

    std::shared_ptr<Texture> cardTex1;
    std::shared_ptr<Texture> cardTex2;

    glm::mat4 projectionMatrix = glm::ortho(0.0f, (float)(WINDOWWIDTH),0.0f, (float)(WINDOWHEIGHT),-1.0f, 1.0f);
    glm::vec3 model1Pos(900.0f, 750.0f, 0.0f);
    glm::vec3 model2Pos(900.0f, 300.0f, 0.0f);
    glm::vec3 cardTexScale(100.0f, 147.0f, 0.0f);

    std::vector<float> vertices{
        -0.5f, -0.5f, 0.0f, 0.0f,
         0.5f, -0.5f, 1.0f, 0.0f,
         0.5f,  0.5f, 1.0f, 1.0f,
        -0.5f,  0.5f, 0.0f, 1.0f,
    };

    std::vector<unsigned int> indices{
        0, 1, 2, 2, 3, 0,
    };

    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2);

    std::unique_ptr<Backend> utils = std::make_unique<Backend>("res/shaders/shader.vert", "res/shaders/shader.frag", layout, vertices, indices);
    std::unique_ptr<ImageManipulation> textureCache = std::make_unique<ImageManipulation>(SUB_SPRITE_WIDTH, SUB_SPRITE_HEIGHT);
    textureCache->LoadTextures("res/textures/test3.png");
    cardTex1 = std::make_shared<Texture>("res/textures/test.png");
    cardTex2 = std::make_shared<Texture>("res/textures/test.png");

    std::vector<std::pair<ImTextureID, std::string>> imGuiIDs;
    for (size_t i = 0; i < textureCache->keys.size(); ++i) {
        int id = textureCache->GetTexIdAt(i);
        if (id < 0) return -1;
        ImTextureID texID = (ImTextureID)(intptr_t)id;
        std::string s = "imageButton" + std::to_string(i);
        imGuiIDs.push_back(std::make_pair(texID, s));
    }

    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    ImGuiIO& io = ImGui::GetIO(); (void)io;


    ImVec2 size(50*textureScale, 74*textureScale);
    ImVec2 uv0_flipped(0.0f, 1.0f);  // upper-left
    ImVec2 uv1_flipped(1.0f, 0.0f);  // lower-right

    GLCall(glClearColor(0.086f, 0.086f, 0.086f, 0.0f));

    // Main Loop
    while (!glfwWindowShouldClose(window)) {
        renderer.Clear();

        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), model1Pos);
            model = glm::scale(model, cardTexScale * textureScale);
            glm::mat4 mvp = projectionMatrix * model;
            textureCache->LoadTextureFromCache(textureCache->keys.at(cardTexIndex1), cardTex1);
            utils->Render(mvp);
        }
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), model2Pos);
            model = glm::scale(model, cardTexScale * textureScale);
            glm::mat4 mvp = projectionMatrix * model;
            textureCache->LoadTextureFromCache(textureCache->keys.at(cardTexIndex2), cardTex2);
            utils->Render(mvp);
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        {
            ImGui::Begin("Debug Menu");
            ImGui::SliderFloat("Scale", &textureScale, 2.0f, 12.0f);
            ImGui::SliderInt("Card 1", &cardTexIndex1, 0, textureCache->keys.size()-1);
            ImGui::DragFloat2("Model 1 position", &model1Pos.x, 1.0f, 0.0f, (float)(WINDOWWIDTH), "%.2f");
            ImGui::SliderInt("Card 2", &cardTexIndex2, 0, textureCache->keys.size()-1);
            ImGui::DragFloat2("Model 2 position", &model2Pos.x, 1.0f, 0.0f, (float)(WINDOWWIDTH), "%.2f");
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }

        {
            ImGui::SetNextWindowSize(ImVec2(635, 225));
            ImGui::Begin("Card 1 Menu", nullptr, ImGuiWindowFlags_HorizontalScrollbar);
            for (size_t i = 0; i < textureCache->keys.size(); ++i) {
                if (ImGui::ImageButton(imGuiIDs[i].second.c_str(), imGuiIDs[i].first, size, uv0_flipped, uv1_flipped)) {
                    cardTexIndex1 = i;
                }
                ImGui::SameLine();
            }
            ImGui::End();
        }

        // {
        //     ImGui::SetNextWindowSize(ImVec2(635,205));
        //     ImGui::Begin("Card 2 Menu", nullptr, ImGuiWindowFlags_HorizontalScrollbar);
        //     for (size_t i = 0; i < textureCache->keys.size(); ++i) {
        //         if (ImGui::ImageButton(imGuiIDs[i].second.c_str(), imGuiIDs[i].first, size, uv0_flipped, uv1_flipped)) {
        //             cardTexIndex1 = i;
        //         }
        //         ImGui::SameLine();
        //     }
        //     ImGui::End();
        // }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
