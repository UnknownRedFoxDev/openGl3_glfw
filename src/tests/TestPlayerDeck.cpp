#include "TestPlayerDeck.h"
#include "GameClasses.h"
#include <memory>
#include <random>
#include <chrono>

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 1080

namespace test {
    TestPlayerDeck::TestPlayerDeck() : io(ImGui::GetIO()){
        (void)io;

        utils = std::make_unique<Backend>("res/shaders/shader.vert", "res/shaders/shader.frag");
        textureCache = std::make_unique<ImageManipulation>(SUB_SPRITE_WIDTH, SUB_SPRITE_HEIGHT);
        textureCache->LoadTextures("res/textures/test3.png");

        // TODO 1: Get GameClasses.h to work
        // TODO 2: Create a Player class
        float posX = WINDOW_WIDTH / 2.0f;
        float pos1Y = WINDOW_HEIGHT / 1.5f + SUB_SPRITE_HEIGHT / 2.0f;
        float pos2Y = WINDOW_HEIGHT / 3.0f - SUB_SPRITE_HEIGHT / 2.0f;
        card1 = std::make_unique<CardObj>(WINDOW_WIDTH, WINDOW_HEIGHT, glm::vec2(posX, pos1Y), glm::vec2(CARD_SIZE_X, CARD_SIZE_Y));
        card2 = std::make_unique<CardObj>(WINDOW_WIDTH, WINDOW_HEIGHT, glm::vec2(posX, pos2Y), glm::vec2(CARD_SIZE_X, CARD_SIZE_Y));


        int index = 0;
        for (unsigned char i = 0; i < 4; ++i) {
            for (unsigned char j = 0; j < 14; ++j) {
                cardMap.emplace(index++, std::make_pair(i, j));
            }
        }
        index = 0;
        for (; index < 28; ++index) {
            if (isCardValid(cardMap.at(index))) {
                first.AddCard(cardMap.at(index));
            }
        }
        for (; index < 55; ++index) {
            if (isCardValid(cardMap.at(index))) {
                second.AddCard(cardMap.at(index));
            }
        }
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::default_random_engine engine(seed);
        // std::shuffle(first.hand.begin(), first.hand.end(), engine);
        first.Update_GUI_Infos(cardMap, textureCache.get());
        sizeCheckDeck1 = first.hand.size();

        // std::shuffle(second.hand.begin(), second.hand.end(), engine);
        second.Update_GUI_Infos(cardMap, textureCache.get());
        sizeCheckDeck2 = second.hand.size();
    }

    void TestPlayerDeck::OnUpdate() {
        if (first.hand.size() != sizeCheckDeck1) first.Update_GUI_Infos(cardMap, textureCache.get());
        sizeCheckDeck1 = first.hand.size();
        if (second.hand.size() != sizeCheckDeck2) second.Update_GUI_Infos(cardMap, textureCache.get());
        sizeCheckDeck2 = second.hand.size();
    }

    void TestPlayerDeck::OnRender() {
        {
            card1->setTexture(textureCache->FetchTextureFromCache(card1->m_TextureIndex));
            utils->Render(card1->getMVP());
        }
        {
            card2->setTexture(textureCache->FetchTextureFromCache(card2->m_TextureIndex));
            utils->Render(card2->getMVP());
        }
    }


    void TestPlayerDeck::OnImGuiRender() {
        {
            ImGui::Begin("Debug Menu Card 1");
            ImGui::DragFloat("Card 1 Scale", &card1->m_TextureScale, 0.1f, 0.0f, 20.0f, "%.1f");
            ImGui::SliderInt("Card 1 Index", &card1->m_TextureIndex, 0, 55);
            ImGui::DragFloat2("Card 1 Pos", &card1->getPosition()->x, 1.0f, 0.0f, (float)(WINDOW_WIDTH), "%.2f");
            ImGui::End();
        }
        {
            ImGui::Begin("Debug Menu Card 2");
            ImGui::DragFloat("Card 2 Scale", &card2->m_TextureScale, 0.1f, 0.0f, 20.0f, "%.1f");
            ImGui::SliderInt("Card 2 Index", &card2->m_TextureIndex, 0, 55);
            ImGui::DragFloat2("Card 2 Pos", &card2->getPosition()->x, 1.0f, 0.0f, (float)(WINDOW_WIDTH), "%.2f");
            ImGui::End();
        }

        if (displayDeck1) first.displayDeck("Card 1", &card1->m_TextureIndex, cardMap, card1->m_TextureScale);
        if (displayDeck2) second.displayDeck("card 2", &card2->m_TextureIndex, cardMap, card2->m_TextureScale);

        ImGui::Checkbox("Display Deck 1", &displayDeck1);
        ImGui::Checkbox("Display Deck 2", &displayDeck2);
        if (ImGui::Button("Remove in first deck")) {
            second.AddCard(first.RemoveCard());
        }
        if (ImGui::Button("Remove in second deck")) {
            first.AddCard(second.RemoveCard());
        }
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    }
}
