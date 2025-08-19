#include <memory>

#include "ImGui/imgui.h"

#include "TestTexture2D.h"
#include "Renderer.h"

namespace test {

    TestTexture2D::TestTexture2D()
        : m_BlendingEnabled(true), m_windowWidth(960), m_windowHeight(540),
        m_ProjectionMatrix(glm::ortho(0.0f, (float)(960),0.0f, (float)(540),-1.0f, 1.0f)),
        m_CameraPos(0, 0, 0), m_ModelPos(300, 200, 0), m_Scale(400.0f, 200.0f, 0.0f)
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
        m_Shader = std::make_unique<Shader>("res/shaders/shader.vert", "res/shaders/shader.frag");
        m_VAO->AddBuffer(*m_VBO, m_Layout);
        m_Shader->Bind();

        m_Texture = std::make_unique<Texture>("res/textures/test.png");
        m_Texture->Bind();
        std::string textureUniform = "uTexture";
        m_Shader->SetUniform1i(textureUniform, 0);
    }

    void TestTexture2D::OnRender()
    {
        Renderer renderer;
        m_Texture->Bind();

        {
            // -x, x, -y, y, -z, z
            glm::mat4 view = glm::translate(glm::mat4(1.0f), m_CameraPos);
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_ModelPos);
            model = glm::scale(model, m_Scale);
            glm::mat4 mvp = m_ProjectionMatrix * view * model;
            m_Shader->Bind();
            m_Shader->SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(*m_VAO, *m_EBO, *m_Shader);
        }
    }

    void TestTexture2D::OnImGuiRender() {
        if (ImGui::Checkbox("Blending Enabled", &m_BlendingEnabled)) {
            GLCall(glEnable(GL_BLEND));
            GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        } else {
            GLCall(glDisable(GL_BLEND));
        }
        ImGui::DragFloat2("Camera position", &m_CameraPos.x, 1.0f, 0.0f, (float)(m_windowWidth), "%.2f");
        ImGui::DragFloat2("Model position", &m_ModelPos.x, 1.0f, 0.0f, (float)(m_windowWidth), "%.2f");
        ImGui::DragFloat2("Model Scale", &m_Scale.x, 0.1f, 0.0f, (float)(m_windowWidth), "%.2f");
    }
}
