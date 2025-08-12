#pragma once

#include <memory>
#include <string>
#include <vector>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Shader.h"

class Backend {
	public:
		struct Geometry {
			public:
				std::vector<float> vertices;
				std::vector<unsigned int> indices;
				std::unique_ptr<VertexArray> m_VAO;
				std::unique_ptr<IndexBuffer> m_EBO;
				VertexBufferLayout m_Layout;

				Geometry(std::vector<float> vertices, std::vector<unsigned int> indices, VertexBufferLayout& layout)
					: vertices(vertices), indices(indices), m_Layout(layout) {
						m_EBO = std::make_unique<IndexBuffer>(indices.data(), indices.size());
						m_VBO = std::make_unique<VertexBuffer>(vertices.data(), vertices.size());
						m_VAO = std::make_unique<VertexArray>();

						m_VAO->AddBuffer(*m_VBO, m_Layout);
					}

			private:
				std::unique_ptr<VertexBuffer> m_VBO;
		};

	public:
		std::unique_ptr<Shader> m_Shader;
		Geometry& m_ObjectPropreties;

		Backend(const std::string& vertPath, const std::string& fragPath, Geometry& geometryInfo);
		void Render(glm::mat4 mvp);
		inline bool IsBlendingEnabled() { return m_BlendingEnabled; }
		inline void SetBlending(bool blendingEnabled) { this->m_BlendingEnabled = blendingEnabled; }

	private:
		bool m_BlendingEnabled;
};

