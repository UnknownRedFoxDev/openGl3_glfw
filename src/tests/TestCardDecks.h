#pragma once
#include "Test.h"
#include <unordered_map>
#include <utility>
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Texture.h"
#include "Pipeline.h"
#include "ImageManipulation.h"
#include <memory>
#include "ImGui/imgui.h"

typedef std::pair<unsigned char, unsigned char> Card;

namespace test {
    class Deck {
        public:
            std::vector<Card> hand;
            std::vector<std::pair<ImTextureID, std::string>> imGuiIDs;

            void AddCard(Card card);
            Card RemoveCard(unsigned int index = 0);
            Card RemoveCard(Card card);
            Card GetCardAt(unsigned int index);
            unsigned int GetCardIndex(unsigned int index, std::unordered_map<unsigned int, Card> cardMap);
            unsigned int GetCardIndex(Card obj, std::unordered_map<unsigned int, Card> cardMap);
            void Update_GUI_Infos(std::unordered_map<unsigned int, Card> cardMap,ImageManipulation* textureCache);

            Deck() {};
    };

    class TestDecks : public Test {
        public:
            TestDecks();
            void OnUpdate() override;
            void OnRender() override;
            void OnImGuiRender() override;
        private:
            Deck main;
            size_t lastSizeDeck;

            glm::mat4 projectionMatrix;
            glm::vec3 model1Pos, model2Pos, cardTexScale;
            std::shared_ptr<Texture> cardTex1;
            std::shared_ptr<Texture> cardTex2;
            std::unique_ptr<Backend> utils;
            std::unique_ptr<ImageManipulation> textureCache;
            int cardTexIndex1 = 0, cardTexIndex2 = 0;
            float textureScale = 2.2f;
            std::unordered_map<unsigned int, Card> cardMap;
            ImVec2 size = ImVec2(50*textureScale, 74*textureScale);
            ImVec2 uv0_flipped = ImVec2(0.0f, 1.0f);  // upper-left
            ImVec2 uv1_flipped = ImVec2(1.0f, 0.0f);  // lower-right
            ImGuiIO& io;
    };
}
