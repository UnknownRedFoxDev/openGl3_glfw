#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "Test.h"
#include "Pipeline.h"
#include "ImageManipulation.h"
#include "Shader.h"
#include "Texture.h"

#include "stb_image/stb_image.h"

typedef unsigned char byte;

namespace test {
    class TestRefactoring2 : public Test {
        public:
            float m_TextureScale;
            int m_CardTexIndex1, m_CardTexIndex2;
            std::shared_ptr<Texture> m_CardTex1;
            std::shared_ptr<Texture> m_CardTex2;
            TestRefactoring2();

            void OnRender() override;
            void OnImGuiRender() override;
        private:
            std::unique_ptr<ImageManipulation> m_Cache;
            int m_windowWidth, m_windowHeight;
            glm::mat4 m_ProjectionMatrix;
            glm::vec3 m_Model1Pos, m_Model2Pos, m_Scale;
            std::unique_ptr<Backend> utils;
    };

}

