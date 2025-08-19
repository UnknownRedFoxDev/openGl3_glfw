#pragma once

#include "Test.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Texture.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <memory>

namespace test {

    class TestTexture2D : public Test {
        public:
            TestTexture2D();

            void OnRender() override;
            void OnImGuiRender() override;

            inline bool IsBlendingEnabled() { return m_BlendingEnabled; }
            inline void SetBlending(bool blendingEnabled) { this->m_BlendingEnabled = blendingEnabled; }

        private:
            bool m_BlendingEnabled;
            int m_windowWidth, m_windowHeight;
            glm::mat4 m_ProjectionMatrix;
            glm::vec3 m_CameraPos, m_ModelPos, m_Scale;
            std::unique_ptr<VertexArray> m_VAO;
            std::unique_ptr<VertexBuffer> m_VBO;
            VertexBufferLayout m_Layout;
            std::unique_ptr<IndexBuffer> m_EBO;
            std::unique_ptr<Shader> m_Shader;
            std::unique_ptr<Texture> m_Texture;
    };

}

