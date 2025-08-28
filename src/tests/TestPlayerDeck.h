#pragma once

#include "Test.h"
#include "GameClasses.h"
#include "Pipeline.h"
#include "ImageManipulation.h"

#include "ImGui/imgui.h"
#include "TestCardObj.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <memory>

namespace test {

    class TestPlayerDeck : public Test {
        public:
            TestPlayerDeck();
            void OnUpdate() override;
            void OnRender() override;
            void OnImGuiRender() override;
        private:
            std::unique_ptr<CardObj> card1;
            Deck first;
            bool displayDeck1 = true;
            size_t sizeCheckDeck1;

            std::unique_ptr<CardObj> card2;
            Deck second;
            bool displayDeck2 = true;
            size_t sizeCheckDeck2;

            int index = 0;

            std::unique_ptr<Backend> utils;
            std::unique_ptr<ImageManipulation> textureCache;
            std::unordered_map<unsigned int, Card> cardMap;
            ImGuiIO& io;
    };
}
