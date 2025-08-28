#include <stdexcept>

#include "TestCardObj.h"
#include "ImageManipulation.h"
#include "ImGui/imgui.h"
#include "Texture.h"

#define SUB_SPRITE_WIDTH 64
#define SUB_SPRITE_HEIGHT 94

namespace test {

    CardObj::CardObj(unsigned int widthWindow, unsigned int heightWindow,
                    glm::vec2 pos, glm::vec2 size, float textureScale)
    {
        m_Texture = std::make_shared<Texture>("res/textures/test.png");
        m_ModelPos = glm::vec3(pos, 0);
        m_ModelSize = glm::vec3(size, 0);
        m_TextureIndex = 0;
        m_TextureScale = textureScale;
        setProjMat(widthWindow, heightWindow);
    }

    CardObj::CardObj(unsigned int widthWindow, unsigned int heightWindow,
                    int posX, int posY,
                    unsigned int sizeX, unsigned int sizeY,
                    float textureScale)
    {
        m_ModelPos = glm::vec3(posX, posY, 0);
        m_ModelSize = glm::vec3(sizeX, sizeY, 0);
        m_TextureScale = textureScale;
        setProjMat(widthWindow, heightWindow);
    }

    glm::mat4 CardObj::getMVP() {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), m_ModelPos);
        model = glm::scale(model, m_ModelSize * m_TextureScale);
        return m_ProjectionMatrix * model;
    }

    void CardObj::setProjMat(float width, float height) {
        if (width <= 0.0f) throw std::runtime_error("setProjMat: width is invalid (<=0.0f)");
        if (height <= 0.0f) throw std::runtime_error("setProjMat: height is invalid (<=0.0f)");
        m_ProjectionMatrix = glm::mat4(glm::ortho(0.0f, width,
                                                  0.0f, height,
                                                  -1.0f, 1.0f
                                                  ));
    }

    TestCardObj::TestCardObj(int width, int height)
        : m_windowWidth(width), m_windowHeight(height)
    {
        utils = std::make_unique<Backend>("res/shaders/shader.vert", "res/shaders/shader.frag");
        m_Cache = std::make_unique<ImageManipulation>(SUB_SPRITE_WIDTH, SUB_SPRITE_HEIGHT);
        m_Cache->LoadTextures("res/textures/test3.png");
        // width / 2 - cardSize / 2
        float posX = m_windowWidth / 2.0f;

        // width / 2 - cardSize / 2
        float pos1Y = m_windowHeight / 1.5f + SUB_SPRITE_HEIGHT / 2.0f;
        float pos2Y = m_windowHeight / 3.0f - SUB_SPRITE_HEIGHT / 2.0f;

        m_Card1 = std::make_unique<CardObj>(m_windowWidth, m_windowHeight, glm::vec2(posX, pos1Y), glm::vec2(100, 147), 2.2f);
        m_Card2 = std::make_unique<CardObj>(m_windowWidth, m_windowHeight, glm::vec2(posX, pos2Y), glm::vec2(100, 147), 2.2f);
    }

    void TestCardObj::OnRender()
    {
        {
            m_Card1->setTexture(m_Cache->FetchTextureFromCache(m_Card1->m_TextureIndex));
            utils->Render(m_Card1->getMVP());
        }
        {
            m_Card2->setTexture(m_Cache->FetchTextureFromCache(m_Card2->m_TextureIndex));
            utils->Render(m_Card2->getMVP());
        }
    }

    void TestCardObj::OnImGuiRender() {
        ImGui::DragFloat("Card 1 Scale", &m_Card1->m_TextureScale, 0.1f, 0.0f, 20.0f, "%.1f");
        ImGui::SliderInt("Card 1 Index", &m_Card1->m_TextureIndex, 0, 55);
        ImGui::DragFloat2("Card 1 Pos", &m_Card1->getPosition()->x, 1.0f, 0.0f, (float)(m_windowWidth), "%.2f");
        ImGui::DragFloat("Card 2 Scale", &m_Card2->m_TextureScale, 0.1f, 0.0f, 20.0f, "%.1f");
        ImGui::SliderInt("Card 2 Index", &m_Card2->m_TextureIndex, 0, 55);
        ImGui::DragFloat2("Card 2 Pos", &m_Card2->getPosition()->x, 1.0f, 0.0f, (float)(m_windowWidth), "%.2f");
    }
}
