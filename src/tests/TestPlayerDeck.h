#pragma once

#include "Test.h"
#include "GameClasses.h"
#include "Pipeline.h"
#include "ImageManipulation.h"

#include "ImGui/imgui.h"
#include "TestCardObj.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Renderer.h"

#include <memory>

namespace test {
    class Player {
        private:
            std::unique_ptr<CardObj> m_CardQuad;
            const std::unordered_map<unsigned int, Card>* cardMapReference;

        public:
            Deck playingDeck;
            Deck waitingDeck;
            bool displayPlayingDeck = true;
            size_t playingDeckSize;
#if DEBUG
            bool displayWaitingDeck = true;
            size_t waitingDeckSize;
#endif //DEBUG

        public:
            Player(const std::unordered_map<unsigned int, Card>* cardMap, int width, int height);
            void UpdateIconCache(const ImageManipulation* textureCache);
            void CheckDeckSize(const ImageManipulation* textureCache);
            inline size_t GetPlayingDeckSize() { return playingDeck.hand.size(); }
            inline size_t GetWaitingDeckSize() { return playingDeck.hand.size(); }
            inline CardObj* GetQuad() { return m_CardQuad.get(); }
    };

    class TestPlayerDeck : public Test {
        public:
            TestPlayerDeck(int width, int height);
            void OnUpdate() override;
            void OnRender() override;
            void OnImGuiRender() override;
        private:
            int width, height;
            std::unique_ptr<Player> m_Player1;
            std::unique_ptr<Player> m_Player2;
            std::unique_ptr<Backend> utils;
            std::unique_ptr<ImageManipulation> textureCache;
            std::shared_ptr<std::unordered_map<unsigned int, Card>> cardMap;
            ImGuiIO& io;
    };
}
