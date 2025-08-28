#include "GameClasses.h"
#include <algorithm>
#include <iostream>
#include <stdexcept>

CardObj::CardObj(unsigned int widthWindow, unsigned int heightWindow,
        glm::vec2 pos, glm::vec2 size, float textureScale)
{
    m_Texture = std::make_shared<Texture>("res/textures/test.png");
    m_ModelPos = glm::vec3(pos, 0);
    m_ModelSize = glm::vec3(size, 0);
    m_TextureIndex = 0;
    m_TextureScale = textureScale;
    setProjMat(widthWindow, heightWindow);
}

CardObj::CardObj(unsigned int widthWindow, unsigned int heightWindow,
        int posX, int posY,
        unsigned int sizeX, unsigned int sizeY,
        float textureScale)
{
    m_ModelPos = glm::vec3(posX, posY, 0);
    m_ModelSize = glm::vec3(sizeX, sizeY, 0);
    m_TextureScale = textureScale;
    setProjMat(widthWindow, heightWindow);
}

glm::mat4 CardObj::getMVP() {
    glm::mat4 model = glm::translate(glm::mat4(1.0f), m_ModelPos);
    model = glm::scale(model, m_ModelSize * m_TextureScale);
    return m_ProjectionMatrix * model;
}

void CardObj::setProjMat(float width, float height) {
    if (width <= 0.0f) throw std::runtime_error("setProjMat: width is invalid (<=0.0f)");
    if (height <= 0.0f) throw std::runtime_error("setProjMat: height is invalid (<=0.0f)");
    m_ProjectionMatrix = glm::mat4(glm::ortho(0.0f, width,
                0.0f, height,
                -1.0f, 1.0f
                ));
}


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

unsigned int Deck::GetCardIndex(unsigned int index, const std::unordered_map<unsigned int, Card> cardMap) const {
    for (auto& card : cardMap) {
        if (card.second == hand.at(index)) {
            return card.first;
        }
    }
    return 0;
}

unsigned int Deck::GetCardIndex(const Card& obj, const std::unordered_map<unsigned int, Card> cardMap) const {
    for (auto& card : cardMap) {
        if (card.second == obj) {
            return card.first;
        }
    }
    return 0;
}

void Deck::displayDeck(const std::string& name,
                       int* cardTexIndex,
                 const std::unordered_map<unsigned int, Card>* cardMap,
                       float textureScale)
{
    // Makes the background of the button the same color as the overall bg
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleColor(ImGuiCol_WindowBg,      ImVec4(0.086f, 0.086f, 0.086f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
    ImGui::SetNextWindowSize(ImVec2(575*(textureScale/2), 185*(textureScale/2)));
    ImGuiInputFlags flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoCollapse;

    ImGui::Begin(name.c_str(), nullptr, flags);
    ImVec2 size = ImVec2(50*textureScale, 74*textureScale);

    // UVs are to reverse the images' UVs because openGL reads them inverted
    ImVec2 UV0 = ImVec2(0.0f, 1.0f);  // upper-left
    ImVec2 UV1 = ImVec2(1.0f, 0.0f);  // lower-right

    for (size_t i = 0; i < hand.size(); ++i) {
        // ImGuiIDs first = Texture ID : second = Texture name/string_id (to differenciate between each button [required by ImGui])
        if (ImGui::ImageButton(imGuiIDs[i].second.c_str(), imGuiIDs[i].first, size, UV0, UV1)) {
            // Card tempCard = hand.at(i);
            // printf("Card Selected = %d:%d\n", tempCard.first, tempCard.second);
            *cardTexIndex = GetCardIndex(i, *cardMap);
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

void Deck::Update_GUI_Infos(const std::unordered_map<unsigned int, Card>* cardMap, const ImageManipulation* textureCache) {
    size_t sizeDeck = hand.size();
    imGuiIDs.clear();
    for (unsigned int i = 0; i < sizeDeck; ++i) {
        int id = textureCache->GetTexIdAt(GetCardIndex(i, *cardMap));
        ImTextureID texID = (ImTextureID)(intptr_t)id;
        std::string s = "imageButton" + std::to_string(i);
        imGuiIDs.push_back(std::make_pair(texID, s));
    }
}
