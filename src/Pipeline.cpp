#include "Pipeline.h"
#include "Renderer.h"

Backend::Backend(const std::string& vertPath,
        const std::string& fragPath)
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

    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2);
    m_Shader = std::make_shared<Shader>(vertPath, fragPath);
    m_EBO = std::make_unique<IndexBuffer>(indices.data(), indices.size());
    m_VBO = std::make_unique<VertexBuffer>(vertices.data(), vertices.size() * sizeof(float));

    m_VAO = std::make_unique<VertexArray>();
    m_VAO->AddBuffer(*m_VBO, layout);
    m_Shader->Bind();

    m_Shader->Bind();
    m_Shader->SetUniform1i("uTexture", 0);
}

void Backend::Render(glm::mat4 mvp)
{
    Renderer renderer;
    m_Shader->Bind();
    m_Shader->SetUniformMat4f("u_MVP", mvp);
    renderer.Draw(*m_VAO, *m_EBO, *m_Shader);
}
