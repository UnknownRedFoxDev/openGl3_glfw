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
        Backend(const std::string& vertPath,
                const std::string& fragPath);
        void Render(glm::mat4 mvp);

    private:
        VertexBufferLayout m_Layout;
        std::unique_ptr<VertexArray> m_VAO;
        std::unique_ptr<VertexBuffer> m_VBO;
        std::unique_ptr<IndexBuffer> m_EBO;
        std::shared_ptr<Shader> m_Shader;
};

