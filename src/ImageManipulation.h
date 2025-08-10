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
		ImageManipulation(int sub_sprite_width, int sub_sprite_height);

		void LoadCache(const std::string& filepath);
		void LoadTextureFromCache(const std::string& key, std::shared_ptr<Texture> tex);
		std::pair<int,int> ParseKey(const std::string& key);
	private:
		std::vector<std::string> keys;
		std::unordered_map<std::string, std::shared_ptr<Texture>> m_Cache;
		int spriteWidth, spriteHeight;
};

