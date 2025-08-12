#include "Pipeline.h"
#include "Renderer.h"

Backend::Backend(const std::string& vertPath, const std::string& fragPath, Geometry& geometryInfo)
	:  m_ObjectPropreties(geometryInfo), m_BlendingEnabled(true)
{
	m_Shader = std::make_unique<Shader>(vertPath, fragPath);
}

void Backend::Render(glm::mat4 mvp)
{
	Renderer renderer;
	m_Shader->Bind();
	m_Shader->SetUniformMat4f("u_MVP", mvp);
	renderer.Draw(*m_ObjectPropreties.m_VAO, *m_ObjectPropreties.m_EBO, *m_Shader);
}

