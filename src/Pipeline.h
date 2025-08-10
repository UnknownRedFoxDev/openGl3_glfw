#pragma once

#include <memory>
#include <string>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Texture.h"

class Backend {
	public:

		Backend(const std::string& vertPath, const std::string& fragPath, const std::string defaultTexPath);
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

