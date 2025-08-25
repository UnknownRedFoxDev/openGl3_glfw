#pragma once

#include "Test.h"
#include "Texture.h"
#include "Pipeline.h"
#include "ImageManipulation.h"

#include "ImGui/imgui.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <cstddef>
#include <memory>
#include <unordered_map>
#include <utility>
#include <vector>

typedef std::pair<unsigned char, unsigned char> Card;

namespace test {
    class Deck {
        public:
            std::vector<Card> hand;
            std::vector<std::pair<ImTextureID, std::string>> imGuiIDs;

            void AddCard(const Card& card);
            Card RemoveCard(const Card& card);
            Card RemoveCard(size_t index = 0);
            unsigned int GetCardIndex(unsigned int index, std::unordered_map<unsigned int, Card> cardMap) const;
            unsigned int GetCardIndex(const Card& obj, std::unordered_map<unsigned int, Card> cardMap) const;
            void Update_GUI_Infos(const std::unordered_map<unsigned int, Card>& cardMap,ImageManipulation* textureCache);

            Deck() {};
    };

    class TestDecks : public Test {
        public:
            TestDecks();
            void OnUpdate() override;
            void OnRender() override;
            void OnImGuiRender() override;
        private:
            Deck first;
            bool displayDeck1 = true;
            Deck second;
            bool displayDeck2 = true;
            size_t sizeCheckDeck1;
            size_t sizeCheckDeck2;
            int index = 0;

            glm::mat4 projectionMatrix;
            glm::vec3 model1Pos, model2Pos, cardTexScale;
            std::shared_ptr<Texture> cardTex1;
            std::shared_ptr<Texture> cardTex2;
            std::unique_ptr<Backend> utils;
            std::unique_ptr<ImageManipulation> textureCache;
            int cardTexIndex1 = 0, cardTexIndex2 = 0;
            float textureScale = 2.2f;
            std::unordered_map<unsigned int, Card> cardMap;
            ImGuiIO& io;
    };
}
