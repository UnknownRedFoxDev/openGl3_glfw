#include <memory>

#include "TestRefactoring2.h"
#include "ImageManipulation.h"
#include "ImGui/imgui.h"
#include "VertexBufferLayout.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Texture.h"

#define SUB_SPRITE_WIDTH 32
#define SUB_SPRITE_HEIGHT 47


namespace test {

    TestRefactoring2::TestRefactoring2()
        : m_TextureScale(1.6f), m_CardTexIndex1(0), m_CardTexIndex2(0), m_windowWidth(1920), m_windowHeight(1080),
        m_ProjectionMatrix(glm::ortho(0.0f, (float)(1920),0.0f, (float)(1080),-1.0f, 1.0f)),
        m_Model1Pos(480.0f, 156.0f, 0.0f), m_Model2Pos(480.0f, 256.0f, 0.0f), m_Scale(100.0f, 147.0f, 0.0f)
    {
        utils = std::make_unique<Backend>("res/shaders/shader.vert", "res/shaders/shader.frag");
        m_Cache = std::make_unique<ImageManipulation>(SUB_SPRITE_WIDTH, SUB_SPRITE_HEIGHT);
        m_Cache->LoadTextures("res/textures/test.png");
        m_CardTex1 = std::make_shared<Texture>("res/textures/test.png");
        m_CardTex2 = std::make_shared<Texture>("res/textures/test.png");
    }

    void TestRefactoring2::OnRender()
    {
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_Model1Pos);
            model = glm::scale(model, m_Scale * m_TextureScale);
            glm::mat4 mvp = m_ProjectionMatrix * model;
            m_Cache->LoadTextureFromCache(m_Cache->keys.at(m_CardTexIndex1), m_CardTex1);
            utils->Render(mvp);
        }
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_Model2Pos);
            model = glm::scale(model, m_Scale * m_TextureScale);
            glm::mat4 mvp = m_ProjectionMatrix * model;
            m_Cache->LoadTextureFromCache(m_Cache->keys.at(m_CardTexIndex2), m_CardTex2);
            utils->Render(mvp);
        }
    }

    void TestRefactoring2::OnImGuiRender() {
        ImGui::DragFloat("Texture Scale", &m_TextureScale, 0.1f, 0.0f, 20.0f, "%.1f");
        ImGui::SliderInt("Card 1", &m_CardTexIndex1, 0, 55);
        ImGui::DragFloat2("Model 1 position", &m_Model1Pos.x, 1.0f, 0.0f, (float)(m_windowWidth), "%.2f");
        ImGui::SliderInt("Card 2", &m_CardTexIndex2, 0, 55);
        ImGui::DragFloat2("Model 2 position", &m_Model2Pos.x, 1.0f, 0.0f, (float)(m_windowWidth), "%.2f");
    }
}
