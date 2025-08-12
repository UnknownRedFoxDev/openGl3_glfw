#include <iostream>
#include <algorithm>
#include <memory>
#include <sstream>

#include "TestRefactoring.h"
#include "Renderer.h"
#include "ImGui/imgui.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Texture.h"

#define SUB_SPRITE_WIDTH 32
#define SUB_SPRITE_HEIGHT 47


namespace test {

    TestRefactoring::TestRefactoring()
        : m_TextureScale(1.2f), m_windowWidth(960), m_windowHeight(540),
        m_ProjectionMatrix(glm::ortho(0.0f, (float)(960),0.0f, (float)(540),-1.0f, 1.0f)),
        m_ModelPos(480.0f, 156.0f, 0.0f), m_Scale(100.0f, 147.0f, 0.0f)
    {
        utils = std::make_unique<BackendTest>("res/shaders/shader.vert", "res/shaders/shader.frag", "res/textures/test.png");
        LoadCache("res/textures/test.png", SUB_SPRITE_WIDTH, SUB_SPRITE_HEIGHT);
    }

    void TestRefactoring::OnRender()
    {
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_ModelPos);
            model = glm::scale(model, m_Scale * m_TextureScale);
            glm::mat4 mvp = m_ProjectionMatrix * model;
            LoadTextureFromCache(keys.at(currentSpriteIndex));
            utils->Render(mvp);

        }
    }

    void TestRefactoring::OnImGuiRender() {
        ImGui::DragFloat2("Model position", &m_ModelPos.x, 1.0f, 0.0f, (float)(m_windowWidth), "%.2f");
        ImGui::SliderInt("Card", &currentSpriteIndex, 0, 55);
        ImGui::DragFloat("Texture Scale", &m_TextureScale, 0.1f, 0.0f, 20.0f, "%.1f");
    }

    void TestRefactoring::LoadCache(const std::string& filepath, int spriteWidth, int spriteHeight) {
        stbi_set_flip_vertically_on_load(1);
        int sheetWidth;
        int sheetHeight;
        int bpp;
        byte* rawData = stbi_load(filepath.c_str(), &sheetWidth, &sheetHeight, &bpp, 4);

        int spritesPerRow = sheetHeight / spriteHeight;
        int spritesPerCol = sheetWidth / spriteWidth;

        // 4
        for (int row = 0; row <= spritesPerRow; ++row) {
            // 13
            for (int col = 0; col < spritesPerCol; ++col) {
                std::vector<byte> data(spriteWidth * spriteHeight * bpp);

                int sprite_x = col * spriteWidth;
                int sprite_y = row * spriteHeight;

                for (int y = 0; y < spriteHeight; ++y) {
                    byte* startPtr = rawData + ((y + sprite_y) * sheetWidth + sprite_x) * bpp;
                    auto dstEnd = data.begin() + y * spriteWidth * bpp;

                    std::copy(startPtr, startPtr + spriteWidth * bpp, dstEnd);
                }
                std::string key = std::to_string(row) + ":" + std::to_string(col);
                m_Cache.emplace(key, std::make_shared<Texture>(std::move(data).data(), spriteWidth, spriteHeight, bpp));
            }
        }
        if (rawData) {
            stbi_image_free(rawData);
        } else {
            std::cout << "\n" << __FILE__ << ":" << __LINE__ << " - Error: Failed to load texture" << std::endl;
            std::cout << stbi_failure_reason() << std::endl;
            exit(1);
        }
        for (auto pair : m_Cache) {
            keys.push_back(pair.first);
        }
        std::sort(keys.begin(), keys.end(), [this](const std::string& a, const std::string& b) {
                auto [color1, value1] = ParseKey(a);
                auto [color2, value2] = ParseKey(b);
                if (color1 == color2) { return value1 < value2; }
                return color1 < color2;
                });
    }
    void TestRefactoring::LoadTextureFromCache(const std::string& key) {
        auto it = m_Cache.find(key);
        if (it != m_Cache.end()){
            utils->SetTexture(it->second);
        } else {
            std::cerr << __FILE__ << ":" << __LINE__ << " - Failed to find sprite: " << key << std::endl;
        }
    }

    std::pair<int,int> TestRefactoring::ParseKey(const std::string& key) {
        std::istringstream ss(key);
        std::string part1, part2;
        if (std::getline(ss, part1, ':') && std::getline(ss, part2)) {
            return {std::stoi(part1), std::stoi(part2)};
        }
        return {0,0}; // fallback if parsing fails
    }

    BackendTest::BackendTest(const std::string& vertPath, const std::string& fragPath, const std::string defaultTexPath)
        : m_BlendingEnabled(true)
    {
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
        m_EBO = std::make_unique<IndexBuffer>(indices, 6);
        m_VBO = std::make_unique<VertexBuffer>(vertices, 4 * 4 * sizeof(float));
        m_Layout.Push<float>(2);
        m_Layout.Push<float>(2);

        m_VAO = std::make_unique<VertexArray>();
        m_Shader = std::make_unique<Shader>(vertPath, fragPath);
        m_VAO->AddBuffer(*m_VBO, m_Layout);
        m_Shader->Bind();

        m_Texture = std::make_shared<Texture>(defaultTexPath);
        m_Shader->SetUniform1i("uTexture", 0);
    }

    void BackendTest::Render(glm::mat4 mvp)
    {
        Renderer renderer;
        m_Texture->Bind();

        {
            m_Shader->Bind();
            m_Shader->SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(*m_VAO, *m_EBO, *m_Shader);
        }
    }
}

