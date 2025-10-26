#pragma once

#include "Test.h"
#include "GameClasses.h"
#include "Pipeline.h"
#include "ImageManipulation.h"

#include "ImGui/imgui.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <memory>
#include <unordered_map>

namespace test {

    class GameDeck {
        private:
            std::unique_ptr<Backend> utils;
            std::unique_ptr<ImageManipulation> textureCache;
            std::shared_ptr<std::unordered_map<unsigned int, Card>> cardMap;

        private:
            void InitCardMap();

        public:
            GameDeck();
            inline std::shared_ptr<Texture> GetTexture(int index) { return textureCache->FetchTextureFromCache(index); }
            inline void Render(glm::mat4 MVP) { utils->Render(MVP); }
            inline std::unordered_map<unsigned int, Card>* GetCardMap() { return cardMap.get(); }
            inline ImageManipulation* GetCache() { return textureCache.get(); }
    };

    class TestGameDeck : public Test {
        public:
            TestGameDeck(int width, int height);
            void OnUpdate() override;
            void OnRender() override;
            void OnImGuiRender() override;
        private:
            int width, height;
            std::unique_ptr<Player> m_Player1;
            std::unique_ptr<Player> m_Player2;
            std::unique_ptr<GameDeck> m_GameDeck;
            ImGuiIO& io;
    };
}
