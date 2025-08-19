#include "ImGui/imgui.h"
#include "iostream"
#include <algorithm>
#include <unordered_map>
#include <utility>
#include "TestCardDecks.h"
#include <random>    // for std::default_random_engine
#include <chrono>    // for std::chrono::system_clock


#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080
#define SUB_SPRITE_WIDTH 64
#define SUB_SPRITE_HEIGHT 94

namespace test {
    inline bool isCardValid(Card card) { return card.second; }
    void Deck::AddCard(Card card) {
        if (card.second)
            hand.push_back(card);
        else
            std::cout << __FILE__ << ":" << __LINE__ << " - [WARNING]: card given was not valid. Card: (" \
                << static_cast<int>(card.first) << ", "<< static_cast<int>(card.second) << ")"<< std::endl;
    }
    Card Deck::RemoveCard(Card card) {
        if (hand.size()) {
            if (!card.second) return std::make_pair(0, 0);
            auto it = std::find(hand.begin(), hand.end(), card);
            if (it != hand.end()) {
                hand.erase(it);
                return card;
            }
        }

        std::cout << __FILE__ << ":" << __LINE__ << " - [WARNING]: could not find card in hand, probably because card was not valid. Card: (" \
            << static_cast<int>(card.first) << ", "<< static_cast<int>(card.second) << ")"<< std::endl;
        return std::make_pair(0, 0);
    }

    Card Deck::RemoveCard(unsigned int index) {
        if (hand.size() && index < hand.size()) {
            auto it = std::find(hand.begin(), hand.end(), hand.at(index));
            if (it != hand.end()) {
                Card removed = *it;
                hand.erase(it);
                return removed;
            }
        }
        std::cout << __FILE__ << ":" << __LINE__ << " - [WARNING]: ";
        if (index < hand.size())
            std::cout << "index is greater than the deck's size" << std::endl;
        else if (!hand.size())
            std::cout << "deck is empty" << std::endl;

        return std::make_pair(0, 0);
    }

    Card Deck::GetCardAt(unsigned int index) {
        return hand.at(index);
    }

    unsigned int Deck::GetCardIndex(unsigned int index, std::unordered_map<unsigned int, Card> cardMap) {
        for (auto& card : cardMap) {
            if (card.second == hand.at(index)) {
                return card.first;
            }
        }
        return 0;
    }

    unsigned int Deck::GetCardIndex(Card obj, std::unordered_map<unsigned int, Card> cardMap) {
        for (auto& card : cardMap) {
            if (card.second == obj) {
                return card.first;
            }
        }
        return 0;
    }

    void Deck::Update_GUI_Infos(std::unordered_map<unsigned int, Card> cardMap, ImageManipulation* textureCache) {
        size_t sizeDeck = hand.size();
        imGuiIDs.clear();
        for (unsigned int i = 0; i < sizeDeck; ++i) {
            int id = textureCache->GetTexIdAt(GetCardIndex(i, cardMap));
            ImTextureID texID = (ImTextureID)(intptr_t)id;
            std::string s = "imageButton" + std::to_string(i);
            imGuiIDs.push_back(std::make_pair(texID, s));
        }
    }

    TestDecks::TestDecks()
        : projectionMatrix(glm::ortho(0.0f, (float)(WINDOW_WIDTH),0.0f, (float)(WINDOW_HEIGHT),-1.0f, 1.0f)),
            model1Pos(900.0f, 750.0f, 0.0f),model2Pos(900.0f, 300.0f, 0.0f),cardTexScale(100.0f, 147.0f, 0.0f),io(ImGui::GetIO())
    {
        (void)io;
        utils = std::make_unique<Backend>("res/shaders/shader.vert", "res/shaders/shader.frag");
        textureCache = std::make_unique<ImageManipulation>(SUB_SPRITE_WIDTH, SUB_SPRITE_HEIGHT);
        textureCache->LoadTextures("res/textures/test3.png");

        cardTex1 = std::make_shared<Texture>("res/textures/test.png");
        cardTex2 = std::make_shared<Texture>("res/textures/test.png");


        unsigned int index = 0;
        for (unsigned char i = 0; i < 4; ++i) {
            for (unsigned char j = 0; j < 14; ++j) {
                cardMap.emplace(index++, std::make_pair(i, j));
            }
        }

        for (int i = 0; i < 26; ++i) {
            if (isCardValid(cardMap.at(i))) {
                main.AddCard(cardMap.at(i));
            }
        }
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::default_random_engine engine(seed);
        std::shuffle(main.hand.begin(), main.hand.end(), engine);
        main.Update_GUI_Infos(cardMap, textureCache.get());
        lastSizeDeck = main.hand.size();
    }


    // TODO: Render Cards of the deck in an ImGui window
    void TestDecks::OnUpdate() {
        lastSizeDeck = main.hand.size();
    }

    void TestDecks::OnRender() {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), model1Pos);
        model = glm::scale(model, cardTexScale * textureScale);
        glm::mat4 mvp = projectionMatrix * model;
        textureCache->LoadTextureFromCache(textureCache->keys.at(cardTexIndex1), cardTex1);
        utils->Render(mvp);
    }

    void TestDecks::OnImGuiRender() {
        {
            ImGui::Begin("Debug Menu");
            ImGui::SliderFloat("Scale", &textureScale, 2.0f, 12.0f);
            ImGui::SliderInt("Card 1", &cardTexIndex1, 0, textureCache->keys.size()-1);
            ImGui::DragFloat2("Model 1 position", &model1Pos.x, 1.0f, 0.0f, (float)(WINDOW_WIDTH), "%.2f");
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }
        {
            if (main.hand.size() != lastSizeDeck) main.Update_GUI_Infos(cardMap, textureCache.get());
            ImGui::SetNextWindowSize(ImVec2(635, 225));
            ImGui::Begin("Card 1 Menu", nullptr, ImGuiWindowFlags_HorizontalScrollbar);
            for (size_t i = 0; i < main.hand.size(); ++i) {
                if (ImGui::ImageButton(main.imGuiIDs[i].second.c_str(), main.imGuiIDs[i].first, size, uv0_flipped, uv1_flipped)) {
                    cardTexIndex1 = main.GetCardIndex(i, cardMap);
                }
                ImGui::SameLine();
            }
            ImGui::End();
        }
        if (ImGui::Button("Remove Card")) {
            main.RemoveCard(0);
        }
    }
}
