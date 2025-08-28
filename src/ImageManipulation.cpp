#include <iostream>
#include <algorithm>
#include <memory>
#include <sstream>
#include <stdexcept>

#include "ImageManipulation.h"
#include "Texture.h"


ImageManipulation::ImageManipulation(int sub_sprite_width, int sub_sprite_height)
    : spriteWidth(sub_sprite_width), spriteHeight(sub_sprite_height)
{
}

void ImageManipulation::LoadTextures(const std::string& filepath) {
    stbi_set_flip_vertically_on_load(1);
    int sheetWidth;
    int sheetHeight;
    int bpp;
    unsigned char* rawData = stbi_load(filepath.c_str(), &sheetWidth, &sheetHeight, &bpp, 4);

    int spritesPerRow = sheetHeight / spriteHeight;
    int spritesPerCol = sheetWidth / spriteWidth;

    for (int row = 0; row < spritesPerRow; row++) {
        for (int col = 0; col < spritesPerCol; ++col) {
            std::vector<unsigned char> data(spriteWidth * spriteHeight * 4);

            int sprite_x = col * spriteWidth;
            int sprite_y = row * spriteHeight;

            for (int y = 0; y < spriteHeight-1; ++y) {
                unsigned char* startPtr = rawData + ((y + sprite_y) * sheetWidth + sprite_x) * 4;

                std::copy(startPtr,
                          startPtr + spriteWidth * 4,
                          data.begin() + y * spriteWidth * 4
                );
            }
            std::string key = std::to_string(row) + ":" + std::to_string(col);
            m_Cache.emplace(key, std::make_shared<Texture>(std::move(data).data(), spriteWidth, spriteHeight, 4));
        }
    }
    if (rawData) {
        stbi_image_free(rawData);
    } else {
        std::cout << "\n" << __FILE__ << ":" << __LINE__ << " - Error: Failed to load texture" << std::endl;
        std::cout << stbi_failure_reason() << std::endl;
        exit(1);
    }
    for (auto pair : m_Cache) {
        keys.push_back(pair.first);
    }
    std::sort(keys.begin(), keys.end(), [this](const std::string& a, const std::string& b) {
            auto [color1, value1] = ParseKey(a);
            auto [color2, value2] = ParseKey(b);
            if (color1 == color2) { return value1 < value2; }
            return color1 < color2;
    });
}
void ImageManipulation::LoadTextureFromCache(const std::string& key, std::shared_ptr<Texture> tex, unsigned int slot) {
    auto it = m_Cache.find(key);
    if (it != m_Cache.end()){
        tex = (it->second);
    } else {
        std::cerr << __FILE__ << ":" << __LINE__ << " - Failed to find sprite: " << key << std::endl;
    }
    tex->Bind(slot);
}
std::shared_ptr<Texture> ImageManipulation::FetchTextureFromCache(unsigned int index) {
    std::string key = keys.at(index);
    auto it = m_Cache.find(key);
    if (it == m_Cache.end()){
        std::string error = static_cast<std::string>(__FILE__) + ":" + std::to_string(__LINE__) + "Failed to find sprite";
        throw std::runtime_error(error.c_str());
    }
    return it->second;
}

std::pair<int,int> ImageManipulation::ParseKey(const std::string& key) {
    std::istringstream ss(key);
    std::string part1, part2;
    if (std::getline(ss, part1, ':') && std::getline(ss, part2)) {
        return {std::stoi(part1), std::stoi(part2)};
    }
    return {0,0}; // fallback if parsing fails
}

int ImageManipulation::GetTexIdAt(unsigned int index) const {
    auto it = m_Cache.find(keys.at(index));
    if (it != m_Cache.end()){
        return it->second->GetTexID();
    }
    return -1;
}
