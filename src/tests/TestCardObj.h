#pragma once

#include <memory>

#include "Test.h"
#include "Pipeline.h"
#include "ImageManipulation.h"
#include "Shader.h"
#include "Texture.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "glm/fwd.hpp"
#include "stb_image/stb_image.h"

typedef unsigned char byte;

namespace test {
    class CardObj {
        private:
            glm::vec3 m_ModelPos;
            glm::vec3 m_ModelSize;
            glm::mat4 m_ProjectionMatrix;
            std::shared_ptr<Texture> m_Texture;

        private:
            void setProjMat(float width, float height);

        public:
            CardObj(unsigned int widthWindow, unsigned int heightWindow,
                    glm::vec2 pos, glm::vec2 size, float textureScale = 2.25);

            CardObj(unsigned int widthWindow, unsigned int heightWindow,
                    int posX, int posY,
                    unsigned int sizeX, unsigned int sizeY,
                    float textureScale = 2.25);
            glm::mat4 getMVP();

            inline void setTexture(std::shared_ptr<Texture> pTexture) { m_Texture = pTexture; m_Texture->Bind(); }
            inline void setPosition(glm::vec3 pos) { m_ModelPos = pos; }
            inline glm::vec3* getPosition() { return &m_ModelPos; }

        public:
            float m_TextureScale;
            int m_TextureIndex;
    };

    class TestCardObj : public Test {
        public:
            TestCardObj(int width, int height);

            void OnRender() override;
            void OnImGuiRender() override;
        private:
            int m_windowWidth, m_windowHeight;
            std::unique_ptr<CardObj> m_Card1;
            std::unique_ptr<CardObj> m_Card2;
            std::unique_ptr<ImageManipulation> m_Cache;
            std::unique_ptr<Backend> utils;
    };

}

