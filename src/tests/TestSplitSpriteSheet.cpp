#include "TestSplitSpriteSheet.h"
#include "../Renderer.h"
#include "../vendor/ImGui/imgui.h"
#include "Texture.h"
// #include "../vendor/ImGui/misc/cpp/imgui_stdlib.h"
#include <algorithm>
#include <memory>
#include <string>
#include <unordered_map>

#define SUB_SPRITE_WIDTH 32
#define SUB_SPRITE_HEIGHT 47

namespace test {

	TestSplitSpriteSheet::TestSplitSpriteSheet()
		: m_BlendingEnabled(true), m_windowWidth(960), m_windowHeight(540),
		m_ProjectionMatrix(glm::ortho(0.0f, (float)(960),0.0f, (float)(540),-1.0f, 1.0f)),
		m_CameraPos(0, 0, 0), m_ModelPos(300, 200, 0), m_Scale(400.0f, 200.0f, 0.0f)
	{
		float vertices[] = {
			-0.5f, -0.5f, 0.0f, 0.0f,
			 0.5f, -0.5f, 1.0f, 0.0f,
			 0.5f,  0.5f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 1.0f,
		};

		uint indices[] = {
			0, 1, 2,
			2, 3, 0
		};
		m_EBO = std::make_unique<IndexBuffer>(indices, 6);
		m_VBO = std::make_unique<VertexBuffer>(vertices, 4 * 4 * sizeof(float));
		m_Layout.Push<float>(2);
		m_Layout.Push<float>(2);

		m_VAO = std::make_unique<VertexArray>();
		m_Shader = std::make_unique<Shader>("res/shaders/shader.vert", "res/shaders/shader.frag");
		m_VAO->AddBuffer(*m_VBO, m_Layout);
		m_Shader->Bind();

		// LoadCache("res/textures/test.png", SUB_SPRITE_WIDTH, SUB_SPRITE_HEIGHT);
		m_Texture = std::make_shared<Texture>("res/textures/test.png");
		std::string textureUniform = "uTexture";
		m_Shader->SetUniform1i(textureUniform, 0);
	}

	void TestSplitSpriteSheet::OnRender()
	{
		Renderer renderer;
		m_Texture->Bind();

		{
			// -x, x, -y, y, -z, z
			glm::mat4 view = glm::translate(glm::mat4(1.0f), m_CameraPos);
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_ModelPos);
			model = glm::scale(model, m_Scale);
			glm::mat4 mvp = m_ProjectionMatrix * view * model;
			m_Shader->Bind();
			m_Shader->SetUniformMat4f("u_MVP", mvp);
			// m_Shader->Bind();
			// m_Shader->SetUniform1i("uTexture", 0);
			renderer.Draw(*m_VAO, *m_EBO, *m_Shader);
		}
	}

	void TestSplitSpriteSheet::OnImGuiRender() {
		if (ImGui::Checkbox("Blending Enabled", &m_BlendingEnabled)) {
			GLCall(glEnable(GL_BLEND));
			GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		} else {
			GLCall(glDisable(GL_BLEND));
		}
		ImGui::DragFloat2("Camera position", &m_CameraPos.x, 1.0f, 0.0f, (float)(m_windowWidth), "%.2f");
		ImGui::DragFloat2("Model position", &m_ModelPos.x, 1.0f, 0.0f, (float)(m_windowWidth), "%.2f");
		ImGui::DragFloat2("Model Scale", &m_Scale.x, 0.1f, 0.0f, (float)(m_windowWidth), "%.2f");
	}

	void TestSplitSpriteSheet::LoadCache(const std::string& filepath, int spriteWidth, int spriteHeight) {
		stbi_set_flip_vertically_on_load(1);
		int sheetWidth;
		int sheetHeight;
		int bpp;
		byte* rawData = stbi_load(filepath.c_str(), &sheetWidth, &sheetHeight, &bpp, 4);

		int spritesPerRow = sheetHeight / spriteHeight;
		int spritesPerCol = sheetWidth / spriteWidth;

		// 4
		for (int row = 0; row < spritesPerRow; ++row) {
			// 13
			for (int col = 0; col < spritesPerCol; ++col) {
				std::vector<byte> data(spriteWidth * spriteHeight * bpp);

				int sprite_x = col * spriteWidth;
				int sprite_y = row * spriteHeight;

				for (int y = 0; y < sprite_y; ++y) {
					// in the rawData, from (the sprite y position + the current y level) on sprite sheet, at the most left of the sprite.
					// (bit per pixel/the number of channels from a byte of data).
					byte* startPtr = rawData + ((y + sprite_y) * sheetWidth + sprite_x) * bpp;
					auto dstEnd = data.begin() + y * spriteWidth * bpp;
					// data copied from left to right on each y level to where it is calculated to be stored at.
					// which increments per y level.
					std::copy(startPtr, startPtr + spriteWidth * bpp, dstEnd);
				}
				// After the scan of the sprite:
				std::string key = "s_" + std::to_string(row) + ":" + std::to_string(col);
				byte* texData = std::move(data).data();
				m_Cache.emplace(key, std::make_shared<Texture>(texData, spriteWidth, spriteHeight, bpp));
			}
		}
		if (rawData) {
			stbi_image_free(rawData);
		} else {
			std::cout << "\n" << __FILE__ << ":" << __LINE__ << " - Error: Failed to load texture" << std::endl;
			std::cout << stbi_failure_reason() << std::endl;
			exit(1);
		}
	}
	void TestSplitSpriteSheet::LoadTextureFromCache(const std::string& key) {
		auto it = m_Cache.find(key);
		if (it != m_Cache.end()){
			m_Texture = it->second;
		} else {
			std::cerr << __FILE__ << ":" << __LINE__ << " - Failed to find sprite: " << key << std::endl;
		}
		m_Texture->Bind();
	}
}
