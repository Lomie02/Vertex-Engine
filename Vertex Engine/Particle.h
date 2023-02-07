#pragma once
#include "glm.hpp"
#include "Shader.h"
#include "glad.h"
#include "Transform.h"
#include "Material.h"

struct Particle
{
	Particle() { m_Velocity = glm::vec2(0, 0); m_LifeTime = 0.0f; }


	glm::vec2 m_Velocity;
	float m_LifeTime;
	Transform transform;
	glm::vec4 material;

};

class ParticleSystem 
{
public:
	ParticleSystem(Shader& shader, Texture2D image, unsigned int amount);
	void Update(float deltaTime, GameObject* _Parent, unsigned int newParticles, glm::vec2 offset);
	void Render();

	std::vector<Particle> m_Particles;

	Texture2D m_Image;
	unsigned int VAO;
private:

	unsigned int m_ParticleAmount = 400;

	Shader m_Shader;


	void WarmUp();

	unsigned int YoungestParticle();
	void Eliminate(Particle &particle, GameObject);
};