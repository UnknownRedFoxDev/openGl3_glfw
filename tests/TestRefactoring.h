#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "Test.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Texture.h"

#include "stb_image/stb_image.h"

typedef unsigned char byte;

namespace test {
    class BackendTest {
        public:

            BackendTest(const std::string& vertPath, const std::string& fragPath, const std::string defaultTexPath);
            void Render(glm::mat4 mvp);
            inline bool IsBlendingEnabled() { return m_BlendingEnabled; }
            inline void SetBlending(bool blendingEnabled) { this->m_BlendingEnabled = blendingEnabled; }
            inline void SetTexture(const std::string& filepath) { m_Texture = std::make_shared<Texture>(filepath); m_Texture->Bind(); }
            inline void SetTexture(std::shared_ptr<Texture> texPtr) { m_Texture = texPtr; m_Texture->Bind(); }

        private:
            bool m_BlendingEnabled;
            VertexBufferLayout m_Layout;
            std::unique_ptr<VertexArray> m_VAO;
            std::unique_ptr<VertexBuffer> m_VBO;
            std::unique_ptr<IndexBuffer> m_EBO;
            std::unique_ptr<Shader> m_Shader;
            std::shared_ptr<Texture> m_Texture;
    };

    class TestRefactoring : public Test {
        public:
            int currentSpriteIndex;
            float m_TextureScale;
            TestRefactoring();

            void OnRender() override;
            void OnImGuiRender() override;

            void LoadCache(const std::string& filepath, int spriteWidth, int spriteHeight);
            void LoadTextureFromCache(const std::string& key);
            std::pair<int,int> ParseKey(const std::string& key);
        private:
            std::vector<std::string> keys;
            std::shared_ptr<Texture> m_Texture;
            std::unordered_map<std::string, std::shared_ptr<Texture>> m_Cache;
            int m_windowWidth, m_windowHeight;
            glm::mat4 m_ProjectionMatrix;
            glm::vec3 m_ModelPos, m_Scale;
            std::unique_ptr<BackendTest> utils;
    };

}


