#pragma once
#include "Graphics.h"
#include "Shader.h"
#include "Texture2D.h"
#include <map>
#include <vector>
#include "Material.h"
#include "GameObject.h"
#include "AdvancedSettings.h"

#define LAYER_MAX 20

enum Layer {
	DefaultLayer = 0,
	layer_01 = 1,
	layer_02 = 2,
	layer_03 = 3,
	user_interface = 4,
	transparent = 5,
	layer_06 = 6,
	layer_07 = 7,
	layer_09 = 8,
	layer_08 = 9,
	layer_10 = 10,
	layer_11 = 11,
	layer_12 = 12,
	layer_13 = 13,
	layer_14 = 14,
	layer_15 = 15,
	layer_16 = 16,
	layer_17 = 17,
	layer_18 = 18,
	layer_19 = 19,
	layer_20 = 20,
};

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

	void DrawSprite(Material& material, glm::vec2 position, glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f, float scale = 1.0f, glm::mat4 per = glm::mat4(1.0f));
	void DrawSprite(GameObject* _object ,Material& material, glm::vec3 position, glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f, float scale = 1.0f, glm::mat4 per = glm::mat4(1.0f));
	void TensionDraw(GameObject* _object ,Material& material, glm::vec2 position, glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f, float scale = 1.0f, glm::mat4 per = glm::mat4(1.0f), int _RenderLayer = 0);

	void DrawLine(glm::vec2 _start, glm::vec2 _end, Material& _mat);

	Shader m_TextShader;
	std::vector<glm::vec2> GetVertices() { return m_Vertices; }
	glm::mat4 GetMatrixModel() { return m_ModelMatrix; }
private:
	glm::mat4 m_ModelMatrix;
	std::vector<glm::vec2> m_Vertices;
	Shader m_Shader;
	unsigned int m_quadVAO;

	void SetUpData();
};