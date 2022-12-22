#pragma once
#include "Graphics.h"
#include "Shader.h"
#include "Texture2D.h"
#include <map>

#include "Material.h"
struct Letters {
	unsigned int TextureID;
	unsigned int Advance;
	glm::ivec2 Size;
	glm::ivec2 Bearing;
};

class Vertex2D
{
public:
	Vertex2D(Shader& shader);
	~Vertex2D();

	std::map<char, Letters> m_Characters;

	void Load(std::string font, unsigned int fontSize);
	void DrawText(std::string text, float x, float y, float scale);

	void DrawSprite(Material& material, glm::vec2 position,
		glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f, glm::mat4 per = glm::mat4(1.0f));

	Shader m_TextShader;
private:
	Shader m_Shader;
	unsigned int m_quadVAO;

	void SetUpData();
};