#pragma once
#include "Graphics.h"
#include "Shader.h"
#include "Texture2D.h"
#include <map>

#include <vector>
#include "Material.h"
#include "GameObject.h"
#include "AdvancedSettings.h"

#include "Volume.h"
#include "Particle.h"
#include "Sprite.h"
#define LAYER_MAX 20

/*==========================================================
* 
	Dylan Smith 2023, ©TropicalGames
* 
	Vertex has 2 renderers that it can use. Both have their pros & cons of course but 1 is better than the other.

	Renderers: 
		- Vertex2D (Default Renderer)
		- Tension2D (Advanced Renderer)

	What are the differences?
				
			Vertex2D is a basic sprite renderer that does not support depth testing or layers & transparency sorting.

			Tension2D is a advanced version of the Vertex2D renderer but offers more. Tension can do Layer based depth testing & has Transparency Layer soting as well as 
			having the option to render the depth test to for debugging. Tension is on the road to being implemented with way more renderering systems.

===========================================================*/

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
	void DrawLine(glm::vec2 _start, glm::vec2 _end, Material& _mat);


	// Tension Functions
	void TensionBoot(Shader& shader) { m_ParticleShader = shader; }
	void TensionDraw(GameObject* _object ,Material& material, glm::vec2 position, glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f, float scale = 1.0f, glm::mat4 per = glm::mat4(1.0f), int _RenderLayer = 0);
	void TensionParticle(ParticleSystem& system);
	void TensionTransparencyPass(std::vector<GameObject*> _list, glm::mat4 per);
	void TensionSprite(Sprite* _sprite, glm::mat4 _pro);

	void Tension_Bind_FrameBuffer();
	void Tension_unBind_FrameBuffer();
	void Tension_Rescale_FrameBuffer(float width,float height);

	void TensionVolume(Volume & _vol);

	Shader m_TextShader;
	std::vector<glm::vec2> GetVertices() { return m_Vertices; }
	glm::mat4 GetMatrixModel() { return m_ModelMatrix; }
private:

	Volume m_VertexVolume;
	glm::mat4 m_ModelMatrix;
	std::vector<glm::vec2> m_Vertices;
	Shader m_Shader;
	Shader m_ParticleShader;
	unsigned int m_quadVAO;

	void SetUpData();
};