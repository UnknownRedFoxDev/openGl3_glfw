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
    class BackendTest1 {
        public:
            struct Geometry {
                public:
                    std::vector<float> vertices;
                    std::vector<unsigned int> indices;
                    VertexBufferLayout m_Layout;
                    std::unique_ptr<VertexArray> m_VAO;
                    std::unique_ptr<VertexBuffer> m_VBO;
                    std::unique_ptr<IndexBuffer> m_EBO;

                    Geometry(std::vector<float> vertices, std::vector<unsigned int> indices, VertexBufferLayout& layout, const std::shared_ptr<Shader>& shader)
                        : vertices(vertices), indices(indices), m_Layout(layout) {
                            m_EBO = std::make_unique<IndexBuffer>(indices.data(), indices.size());
                            m_VBO = std::make_unique<VertexBuffer>(vertices.data(), vertices.size() * sizeof(float));

                            m_VAO = std::make_unique<VertexArray>();
                            m_VAO->AddBuffer(*m_VBO, m_Layout);
                            shader->Bind();
                        }
            };

        public:

            BackendTest1(const std::string& vertPath, const std::string& fragPath, const std::string defaultTexPath);
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
            std::shared_ptr<Shader> m_Shader;
            std::shared_ptr<Texture> m_Texture;
            std::unique_ptr<Geometry> m_ObjInfo;
    };

    class TestBatchRenderingTextures : public Test {
        public:
            float m_TextureScale;
            int m_CardTexIndex1, m_CardTexIndex2;
            std::shared_ptr<Texture> m_CardTex1;
            std::shared_ptr<Texture> m_CardTex2;
            TestBatchRenderingTextures();

            void OnRender() override;
            void OnImGuiRender() override;

            void LoadCache(const std::string& filepath, int spriteWidth, int spriteHeight);
            void LoadTextureFromCache(const std::string& key, std::shared_ptr<Texture> tex, unsigned int slot = 0);
            std::pair<int,int> ParseKey(const std::string& key);
        private:
            std::vector<std::string> keys;
            std::shared_ptr<Texture> m_Texture;
            std::unordered_map<std::string, std::shared_ptr<Texture>> m_Cache;
            int m_windowWidth, m_windowHeight;
            glm::mat4 m_ProjectionMatrix;
            glm::vec3 m_Model1Pos, m_Model2Pos, m_Scale;
            std::unique_ptr<BackendTest1> utils;
    };

}
