#include "Particle.h"
#include "Random.h"
ParticleSystem::ParticleSystem(Shader& shader, Texture2D image, unsigned int amount)
{
	unsigned int VBO;
    float quad[] = {
          0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };

    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(this->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

    for (int i = 0; i < this->m_MaxParticles; i++) {
        this->m_Particles.push_back(Particle());
    }
}

void ParticleSystem::Update(float deltaTime, GameObject& _Parent, unsigned int newParticles, glm::vec2 offset)
{
    for (unsigned int i = 0; i < newParticles; i++) {
        int unused = this->YoungestParticle();
        this->RespawnParticle(this->m_Particles[unused], _Parent, offset);
    }
    for (unsigned int i = 0; i < this->m_ParticleAmount; i++) {
        Particle& p = this->m_Particles[i];
        p.m_LifeTime -= deltaTime;
        if (p.m_LifeTime < 0.0f) {
            p.transform.position -= p.m_Velocity * deltaTime;
            p.material.a -= deltaTime * 2.5f;
        }
    }
}

void ParticleSystem::Render()
{
}

void ParticleSystem::WarmUp()
{
}

unsigned int ParticleSystem::YoungestParticle()
{
    return 0;
}

void ParticleSystem::Eliminate(Particle& particle, GameObject)
{
}

void ParticleSystem::RespawnParticle(Particle& _particle, GameObject& _targert, glm::vec2 _offset)
{
    float random = Random::RandomFloatRange(50, 100) / 2;
    float rColour = 0.5f + Random::RandomFloatRange(0, 100) / 100;
    _particle.transform.position = _targert.transform->position, +random + _offset;
    _particle.material = glm::vec4(rColour, rColour, rColour, 1.0f);
    _particle.m_LifeTime = 1.0f;
}
