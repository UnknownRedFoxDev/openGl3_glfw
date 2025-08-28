#include "TestCardDecks.h"
#include "ImGui/imgui.h"
#include <algorithm>
#include <unordered_map>
#include <random>
#include <chrono>

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080
#define SUB_SPRITE_WIDTH 64
#define SUB_SPRITE_HEIGHT 94

namespace test {
    void displayDeck(const std::string& name, const Deck* deck, int* cardTexIndex, const std::unordered_map<unsigned int, Card>& cardMap, float textureScale) {
        // Makes the background of the button the same color as the overall bg
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleColor(ImGuiCol_WindowBg,      ImVec4(0.086f, 0.086f, 0.086f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
        ImGui::SetNextWindowSize(ImVec2(575*(textureScale/2), 185*(textureScale/2)));
        ImGuiInputFlags flags = ImGuiWindowFlags_NoResize               |
                                ImGuiWindowFlags_NoTitleBar             |
                                ImGuiWindowFlags_HorizontalScrollbar    |
                                ImGuiWindowFlags_NoCollapse             ;

        ImGui::Begin(name.c_str(), nullptr, flags);
        ImVec2 size = ImVec2(50*textureScale, 74*textureScale);

        // UVs are to reverse the images' UVs because openGL reads them inverted
        ImVec2 UV0 = ImVec2(0.0f, 1.0f);  // upper-left
        ImVec2 UV1 = ImVec2(1.0f, 0.0f);  // lower-right

        for (size_t i = 0; i < deck->hand.size(); ++i) {
            // ImGuiIDs first = Texture ID : second = Texture name/string_id (to differenciate between each button [required by ImGui])
            if (ImGui::ImageButton(deck->imGuiIDs[i].second.c_str(), deck->imGuiIDs[i].first, size, UV0, UV1)) {
                // Card tempCard = deck->hand.at(i);
                // printf("Card Selected = %d:%d\n", tempCard.first, tempCard.second);
                *cardTexIndex = deck->GetCardIndex(i, cardMap);
            }
            ImGui::SameLine();
        }
        ImGui::End();
        ImGui::PopStyleVar();
        ImGui::PopStyleVar();
        ImGui::PopStyleVar();
        ImGui::PopStyleColor();
        ImGui::PopStyleColor();
        ImGui::PopStyleColor();
        ImGui::PopStyleColor();
    }

    bool isCardValid(Card card) { return card.second; }

    void Deck::AddCard(const Card& card) {
        if (isCardValid(card)) hand.push_back(card);
        else std::cout << "[WARNING]: card given was not valid. Card: ("
            << static_cast<int>(card.first) << ", "<< static_cast<int>(card.second) << ") - "
            << __FILE__ << ":" << __LINE__ <<  std::endl;
    }

    Card Deck::RemoveCard(const Card& card) {
        auto it = std::find(hand.begin(), hand.end(), card);
        if (it == hand.end()) {
            std::cout << "[WARNING]: card given was not found. Card: ("
                << static_cast<int>(card.first) << ", "<< static_cast<int>(card.second) << ") - "
                << __FILE__ << ":" << __LINE__ << std::endl;
            return std::make_pair(0, 0);
        }
        Card removed = *it;
        hand.erase(it);
        return removed;
    }


    Card Deck::RemoveCard(size_t index) {
        if (index >= hand.size()) {
            std::cout << "[WARNING]: Index invalid or Deck empty. deck.size() = " << hand.size() << " - " << __FILE__ << ":" << __LINE__ << std::endl;
            return std::make_pair(0, 0);
        }
        Card removed = hand[index];
        hand.erase(hand.begin() + index);
        return removed;
    }

    unsigned int Deck::GetCardIndex(unsigned int index, std::unordered_map<unsigned int, Card> cardMap) const {
        for (auto& card : cardMap) {
            if (card.second == hand.at(index)) {
                return card.first;
            }
        }
        return 0;
    }

    unsigned int Deck::GetCardIndex(const Card& obj, std::unordered_map<unsigned int, Card> cardMap) const {
        for (auto& card : cardMap) {
            if (card.second == obj) {
                return card.first;
            }
        }
        return 0;
    }

    void Deck::Update_GUI_Infos(const std::unordered_map<unsigned int, Card>& cardMap, ImageManipulation* textureCache) {
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
            model1Pos(900.0f, 750.0f, 0.0f), model2Pos(900.0f, 300.0f, 0.0f),cardTexScale(100.0f, 147.0f, 0.0f),io(ImGui::GetIO())
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
        int i = 0;
        for (; i < 28; ++i) {
            if (isCardValid(cardMap.at(i))) {
                first.AddCard(cardMap.at(i));
            }
        }
        for (; i < 55; ++i) {
            if (isCardValid(cardMap.at(i))) {
                second.AddCard(cardMap.at(i));
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

    void TestDecks::OnUpdate() {
        if (first.hand.size() != sizeCheckDeck1) first.Update_GUI_Infos(cardMap, textureCache.get());
        sizeCheckDeck1 = first.hand.size();
        if (second.hand.size() != sizeCheckDeck2) second.Update_GUI_Infos(cardMap, textureCache.get());
        sizeCheckDeck2 = second.hand.size();
    }

    void TestDecks::OnRender() {
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), model1Pos);
            model = glm::scale(model, cardTexScale * textureScale);
            glm::mat4 mvp = projectionMatrix * model;
            textureCache->LoadTextureFromCache(textureCache->keys.at(cardTexIndex1), cardTex1);
            utils->Render(mvp);
        }
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), model2Pos);
            model = glm::scale(model, cardTexScale * textureScale);
            glm::mat4 mvp = projectionMatrix * model;
            textureCache->LoadTextureFromCache(textureCache->keys.at(cardTexIndex2), cardTex2);
            utils->Render(mvp);
        }
    }


    void TestDecks::OnImGuiRender() {
        {
            ImGui::Begin("Debug Menu Card 1");
            ImGui::SliderInt("Index", &cardTexIndex1, 0, textureCache->keys.size()-1);
            ImGui::DragFloat2("Model position", &model1Pos.x, 1.0f, 0.0f, (float)(WINDOW_WIDTH), "%.2f");
            ImGui::End();
        }
        {
            ImGui::Begin("Debug Menu Card 2");
            ImGui::SliderInt("Index", &cardTexIndex2, 0, textureCache->keys.size()-1);
            ImGui::DragFloat2("Model position", &model2Pos.x, 1.0f, 0.0f, (float)(WINDOW_WIDTH), "%.2f");
            ImGui::End();
        }

        if (displayDeck1) displayDeck("Card 1", &first, &cardTexIndex1, cardMap, textureScale);
        if (displayDeck2) displayDeck("card 2", &second, &cardTexIndex2, cardMap, textureScale);

        ImGui::Checkbox("Display Deck 1", &displayDeck1);
        ImGui::Checkbox("Display Deck 2", &displayDeck2);
        ImGui::SliderFloat("Scale", &textureScale, 2.0f, 2.35f);
        ImGui::SliderInt("Index", &index, 0, 24);
        if (ImGui::Button("Remove in first deck")) {
            second.AddCard(first.RemoveCard((size_t)index));
        }
        if (ImGui::Button("Remove in second deck")) {
            first.AddCard(second.RemoveCard((size_t)index));
        }
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    }
}
