#include "TestGameDeck.h"
#include "GameClasses.h"
#include <algorithm>
#include <memory>
#include <random>
#include <chrono>

namespace test {

    GameDeck::GameDeck() {
        utils = std::make_unique<Backend>("res/shaders/shader.vert", "res/shaders/shader.frag");
        textureCache = std::make_unique<ImageManipulation>(SUB_SPRITE_WIDTH, SUB_SPRITE_HEIGHT);
        textureCache->LoadTextures("res/textures/test3.png");
        cardMap = std::make_shared<std::unordered_map<unsigned int, Card>>();
        InitCardMap();
    }

    void GameDeck::InitCardMap() {
        int index = 0;
        for (unsigned char i = 0; i < 4; ++i) {
            for (unsigned char j = 0; j < 14; ++j) {
                cardMap->emplace(index++, std::make_pair(i, j));
            }
        }
    }

    TestGameDeck::TestGameDeck(int width, int height) : width(width), height(height), io(ImGui::GetIO()){
        (void)io;

        m_GameDeck = std::make_unique<GameDeck>();
        m_Player1 = std::make_unique<Player>(m_GameDeck->GetCardMap(), width, height);
        m_Player2 = std::make_unique<Player>(m_GameDeck->GetCardMap(), width, height);

        float posX = width / 2.0f;
        float pos1Y = height / 1.5f + SUB_SPRITE_HEIGHT / 2.0f;
        float pos2Y = height / 3.0f - SUB_SPRITE_HEIGHT / 2.0f;
        m_Player1->GetQuad()->setPosition(glm::vec3(posX, pos1Y, 0));
        m_Player2->GetQuad()->setPosition(glm::vec3(posX, pos2Y, 0));


        int index = 0;
        for (; index < 28; ++index) {
            if (Deck::IsCardValid(m_GameDeck->GetCardMap()->at(index))) {
                m_Player1->playingDeck.AddCard(m_GameDeck->GetCardMap()->at(index));
            }
        }
        for (; index < 55; ++index) {
            if (Deck::IsCardValid(m_GameDeck->GetCardMap()->at(index))) {
                m_Player2->playingDeck.AddCard(m_GameDeck->GetCardMap()->at(index));
            }
        }
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::default_random_engine engine(seed);
        std::shuffle(m_Player1->playingDeck.hand.begin(), m_Player1->playingDeck.hand.end(), engine);
        std::shuffle(m_Player2->playingDeck.hand.begin(), m_Player2->playingDeck.hand.end(), engine);
    }

    void TestGameDeck::OnUpdate() {
        m_Player1->CheckDeckSize(m_GameDeck->GetCache());
        m_Player2->CheckDeckSize(m_GameDeck->GetCache());
    }

    void TestGameDeck::OnRender() {
        {
            m_Player1->GetQuad()->setTexture(m_GameDeck->GetTexture(m_Player1->GetQuad()->m_TextureIndex));
            m_GameDeck->Render(m_Player1->GetQuad()->getMVP());
        }
        {
            m_Player2->GetQuad()->setTexture(m_GameDeck->GetTexture(m_Player2->GetQuad()->m_TextureIndex));
            m_GameDeck->Render(m_Player2->GetQuad()->getMVP());
        }
    }


    void TestGameDeck::OnImGuiRender() {
        {
            ImGui::Begin("Debug Menu Card 1");
            ImGui::SliderInt("Card 1 Index", &m_Player1->GetQuad()->m_TextureIndex, 0, 55);
            ImGui::DragFloat2("Card 1 Pos",  &m_Player1->GetQuad()->getPosition()->x, 1.0f, 0.0f, (float)(width), "%.2f");
            ImGui::End();
        }
        {
            ImGui::Begin("Debug Menu Card 2");
            ImGui::SliderInt("Card 2 Index", &m_Player2->GetQuad()->m_TextureIndex, 0, 55);
            ImGui::DragFloat2("Card 2 Pos",  &m_Player2->GetQuad()->getPosition()->x, 1.0f, 0.0f, (float)(width), "%.2f");
            ImGui::End();
        }

        if (m_Player1->displayPlayingDeck)
            m_Player1->playingDeck.displayDeck("Playing Deck P1", &m_Player1->GetQuad()->m_TextureIndex, m_GameDeck->GetCardMap(), m_Player1->GetQuad()->m_TextureScale);

        if (m_Player2->displayPlayingDeck)
            m_Player2->playingDeck.displayDeck("Playing Deck P2", &m_Player2->GetQuad()->m_TextureIndex, m_GameDeck->GetCardMap(), m_Player2->GetQuad()->m_TextureScale);

#ifdef DEBUG
        if (m_Player1->displayWaitingDeck)
            m_Player1->waitingDeck.displayDeck("Waiting Deck P1", &m_Player1->GetQuad()->m_TextureIndex, m_GameDeck->GetCardMap(), m_Player1->GetQuad()->m_TextureScale);
        if (m_Player2->displayWaitingDeck)
            m_Player2->waitingDeck.displayDeck("Waiting Deck P2", &m_Player2->GetQuad()->m_TextureIndex, m_GameDeck->GetCardMap(), m_Player2->GetQuad()->m_TextureScale);
#endif // DEBUG
        ImGui::Checkbox("Display Waiting P1", &m_Player1->displayWaitingDeck);
        ImGui::Checkbox("Display Waiting P2", &m_Player2->displayWaitingDeck);
        ImGui::Checkbox("Display Playing P1", &m_Player1->displayPlayingDeck);
        ImGui::Checkbox("Display Playing P2", &m_Player2->displayPlayingDeck);
        if (ImGui::Button("Remove in first deck")) {
            m_Player1->waitingDeck.AddCard(m_Player1->playingDeck.RemoveCard());
        }
        if (ImGui::Button("Remove in second deck")) {
            m_Player2->waitingDeck.AddCard(m_Player2->playingDeck.RemoveCard());
        }
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    }
}
