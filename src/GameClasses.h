#pragma once

#include "Shader.h"
#include "Texture.h"
#include "ImageManipulation.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ImGui/imgui.h"
#include <memory>

#ifndef SUB_SPRITE_WIDTH
    #define SUB_SPRITE_WIDTH 64
#endif //SUB_SPRITE_WIDTH

#ifndef SUB_SPRITE_HEIGHT
    #define SUB_SPRITE_HEIGHT 94
#endif //SUB_SPRITE_HEIGHT

#ifndef CARD_SIZE_X
    #define CARD_SIZE_X 100
#endif //CARD_SIZE_X

#ifndef CARD_SIZE_Y
    #define CARD_SIZE_Y 147
#endif //CARD_SIZE_Y

#ifndef Card
    typedef std::pair<unsigned char, unsigned char> Card;
#endif //Card


inline bool isCardValid(const Card card) { return card.second; }

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



class Deck {
    public:
        std::vector<Card> hand;
        std::vector<std::pair<ImTextureID, std::string>> imGuiIDs;

        void AddCard(const Card& card);
        Card RemoveCard(const Card& card);
        Card RemoveCard(size_t index = 0);
        unsigned int GetCardIndex(unsigned int index, const std::unordered_map<unsigned int, Card> cardMap) const;
        unsigned int GetCardIndex(const Card& obj, const std::unordered_map<unsigned int, Card> cardMap) const;
        void displayDeck(const std::string& name,
                         int* cardTexIndex,
                         const std::unordered_map<unsigned int, Card>* cardMap,
                         float textureScale);
        void Update_GUI_Infos(const std::unordered_map<unsigned int, Card>* cardMap, const ImageManipulation* textureCache);
        inline bool IsEmpty() { return hand.empty(); }

        Deck() {};
};
