#include "Pipeline.h"
#include "Renderer.h"

Backend::Backend(const std::string& vertPath, const std::string& fragPath, const std::string defaultTexPath)
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

void Backend::Render(glm::mat4 mvp)
{
	Renderer renderer;
	m_Texture->Bind();

	{
		m_Shader->Bind();
		m_Shader->SetUniformMat4f("u_MVP", mvp);
		renderer.Draw(*m_VAO, *m_EBO, *m_Shader);
	}
}

