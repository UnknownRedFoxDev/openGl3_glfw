#include <iostream>
#include <algorithm>
#include <memory>
#include <sstream>

#include "TestBatchRenderingTextures.h"
#include "Renderer.h"
#include "ImGui/imgui.h"
#include "VertexBufferLayout.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Texture.h"

#define SUB_SPRITE_WIDTH 32
#define SUB_SPRITE_HEIGHT 47


namespace test {

    TestBatchRenderingTextures::TestBatchRenderingTextures()
        : m_TextureScale(1.6f), m_CardTexIndex1(0), m_CardTexIndex2(0), m_windowWidth(1920), m_windowHeight(1080),
        m_ProjectionMatrix(glm::ortho(0.0f, (float)(1920),0.0f, (float)(1080),-1.0f, 1.0f)),
        m_Model1Pos(600.0f, 628.0f, 0.0f), m_Model2Pos(600.0f, 256.0f, 0.0f), m_Scale(100.0f, 147.0f, 0.0f)
    {
        utils = std::make_unique<BackendTest1>("res/shaders/shader.vert", "res/shaders/shader.frag", "res/textures/test.png");
        LoadCache("res/textures/test.png", SUB_SPRITE_WIDTH, SUB_SPRITE_HEIGHT);
        m_CardTex1 = std::make_shared<Texture>("res/textures/test.png");
        m_CardTex2 = std::make_shared<Texture>("res/textures/test.png");
    }

    void TestBatchRenderingTextures::OnRender()
    {
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_Model1Pos);
            model = glm::scale(model, m_Scale * m_TextureScale);
            glm::mat4 mvp = m_ProjectionMatrix * model;
            LoadTextureFromCache(keys.at(m_CardTexIndex1), m_CardTex1);
            utils->Render(mvp);
        }
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_Model2Pos);
            model = glm::scale(model, m_Scale * m_TextureScale);
            glm::mat4 mvp = m_ProjectionMatrix * model;
            LoadTextureFromCache(keys.at(m_CardTexIndex2), m_CardTex2);
            utils->Render(mvp);
        }
    }

    void TestBatchRenderingTextures::OnImGuiRender() {
        ImGui::DragFloat("Texture Scale", &m_TextureScale, 0.1f, 0.0f, 20.0f, "%.1f");
        ImGui::SliderInt("Card 1", &m_CardTexIndex1, 0, 55);
        ImGui::DragFloat2("Model 1 position", &m_Model1Pos.x, 1.0f, 0.0f, (float)(m_windowWidth), "%.2f");
        ImGui::SliderInt("Card 2", &m_CardTexIndex2, 0, 55);
        ImGui::DragFloat2("Model 2 position", &m_Model2Pos.x, 1.0f, 0.0f, (float)(m_windowWidth), "%.2f");
    }

    void TestBatchRenderingTextures::LoadCache(const std::string& filepath, int spriteWidth, int spriteHeight) {
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

                // going from down to up, it will try to reach to uncharted memory when at the top row of the spritesheet
                for (int y = 0; y < spriteHeight-1; ++y) {
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
    void TestBatchRenderingTextures::LoadTextureFromCache(const std::string& key, std::shared_ptr<Texture> tex, unsigned int slot) {
        auto it = m_Cache.find(key);
        if (it != m_Cache.end()){
            tex = (it->second);
        } else {
            std::cerr << __FILE__ << ":" << __LINE__ << " - Failed to find sprite: " << key << std::endl;
        }
        tex->Bind(slot);
    }

    std::pair<int,int> TestBatchRenderingTextures::ParseKey(const std::string& key) {
        std::istringstream ss(key);
        std::string part1, part2;
        if (std::getline(ss, part1, ':') && std::getline(ss, part2)) {
            return {std::stoi(part1), std::stoi(part2)};
        }
        return {0,0}; // fallback if parsing fails
    }

    BackendTest1::BackendTest1(const std::string& vertPath, const std::string& fragPath, const std::string defaultTexPath)
        : m_BlendingEnabled(true)
    {
        std::vector<float> vertices{
            -0.5f, -0.5f, 0.0f, 0.0f,
                0.5f, -0.5f, 1.0f, 0.0f,
                0.5f,  0.5f, 1.0f, 1.0f,
                -0.5f,  0.5f, 0.0f, 1.0f,
        };

        std::vector<unsigned int> indices{
            0, 1, 2, 2, 3, 0,
        };

        m_Layout.Push<float>(2);
        m_Layout.Push<float>(2);

        m_Shader = std::make_shared<Shader>(vertPath, fragPath);
        m_ObjInfo = std::make_unique<Geometry>(vertices, indices, m_Layout, m_Shader);

        // int samplers[2] = { 0, 1 };
        // m_Shader->SetUniform1iv("uTextures", 2, samplers);
        m_Texture = std::make_shared<Texture>(defaultTexPath);
        m_Shader->Bind();
        m_Shader->SetUniform1i("uTexture", 0);
    }

    void BackendTest1::Render(glm::mat4 mvp)
    {
        Renderer renderer;
        m_Shader->Bind();
        m_Shader->SetUniformMat4f("u_MVP", mvp);
        renderer.Draw(*m_ObjInfo->m_VAO, *m_ObjInfo->m_EBO, *m_Shader);
    }
}

