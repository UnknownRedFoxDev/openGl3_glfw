#pragma once

#include <vector>
#include <memory>
#include <string>
#include <unordered_map>

#include "Texture.h"
#include "stb_image/stb_image.h"

class ImageManipulation {
    public:
        int currentSpriteIndex;
        float m_TextureScale;
        std::vector<std::string> keys;
        ImageManipulation(int sub_sprite_width, int sub_sprite_height);

        void LoadTextures(const std::string& filepath);
        void LoadTextureFromCache(const std::string& key, std::shared_ptr<Texture> tex, unsigned int slot = 0);
        std::shared_ptr<Texture> FetchTextureFromCache(unsigned int index);
        std::pair<int,int> ParseKey(const std::string& key);

        int GetTexIdAt(unsigned int index) const;

    private:
        std::unordered_map<std::string, std::shared_ptr<Texture>> m_Cache;
        int spriteWidth, spriteHeight;
};

