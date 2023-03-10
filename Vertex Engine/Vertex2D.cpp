#include "Vertex2D.h"
Vertex2D::Vertex2D(Shader& shader)
{
	this->m_Shader = shader;
	this->SetUpData();
}

Vertex2D::~Vertex2D()
{
	glDeleteVertexArrays(1, &this->m_quadVAO);
}

void Vertex2D::DrawSprite(Material& material, glm::vec2 position, glm::vec2 size, float rotate, float scale, glm::mat4 per)
{
	glEnable(GL_BLEND);
	glEnable(GL_CULL_FACE);

	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	if (material.surface == Transparent)
	{
	}
	//glDepthFunc(GL_EQUAL);


	this->m_Shader = material.shader;

	this->m_Shader.Use();
	glm::mat4 model = glm::mat4(1.0f);

	model = glm::translate(model, glm::vec3(position, 0));
	model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * -size.y, 0.0f));
	model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * -size.y, 0.0f));

	model = glm::scale(model, glm::vec3(size.x * scale, -size.y * scale, 1.0f));
	this->m_Shader.SetMatrix4("model", model);
	this->m_Shader.SetMatrix4("pro", per);
	this->m_Shader.SetVector4f("Colour", material.colour);

	glActiveTexture(GL_TEXTURE0);
	material.baseTexture.Bind();

	glBindVertexArray(this->m_quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glDisable(GL_CULL_FACE);
}

void Vertex2D::DrawSprite(GameObject* _object, Material& material, glm::vec3 position, glm::vec2 size, float rotate, float scale, glm::mat4 per)
{
	glEnable(GL_BLEND);
	glEnable(GL_CULL_FACE);

	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	this->m_Shader = material.shader;

	this->m_Shader.Use();
	glm::mat4 model = glm::mat4(1.0f);

	model = glm::translate(model, glm::vec3(position));

	if (_object->GetParent() != nullptr)
	{
		model = glm::translate(model, glm::vec3(_object->GetParent()->transform.position.x, _object->GetParent()->transform.position.y, 0.0f));
		model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * -size.y, 0.0f));

		model = glm::scale(model, glm::vec3(size.x * scale, -size.y * scale, 1.0f));
	}
	else
	{
		model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * -size.y, 0.0f));
		model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * -size.y, 0.0f));

		model = glm::scale(model, glm::vec3(size.x * scale, -size.y * scale, 1.0f));
	}

	this->m_Shader.SetMatrix4("model", model);
	this->m_Shader.SetMatrix4("pro", per);
	this->m_Shader.SetVector4f("Colour", material.colour);

	glActiveTexture(GL_TEXTURE0);
	material.baseTexture.Bind();

	glBindVertexArray(this->m_quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glDisable(GL_CULL_FACE);
}

void Vertex2D::TensionDraw(GameObject* _object, Material& material, glm::vec2 position, glm::vec2 size, float rotate, float scale, glm::mat4 per, int _RenderLayer)
{

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	if (material.surface == Transparent)
	{
		glCullFace(GL_BACK);
		glEnable(GL_BLEND);

		if (!material.glow) {
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
		else {
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		}
	}
	else 
	{
		glDepthFunc(GL_LESS);
	}
	this->m_Shader = material.shader;

	this->m_Shader.Use();
	
	if (RENDER_DEPTH_TEST == false) {
		this->m_Shader.SetInteger("UseDepth", 0);
	}
	else {

		this->m_Shader.SetInteger("UseDepth", 1);
	}

	glm::mat4 model = glm::mat4(1.0f);

	model = glm::translate(model, glm::vec3(position, (float)_RenderLayer));

	model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * -size.y, 0.0f));
	model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * -size.y, 0.0f));

	model = glm::scale(model, glm::vec3(size.x * scale, -size.y * scale, 1.0f));
	this->m_Shader.SetMatrix4("model", model);
	this->m_Shader.SetMatrix4("pro", per);
	this->m_Shader.SetVector4f("Colour", material.colour);

	glActiveTexture(GL_TEXTURE0);
	material.baseTexture.Bind();

	glBindVertexArray(this->m_quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glDisable(GL_CULL_FACE);
}

void Vertex2D::TensionParticle(ParticleSystem &system)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	this->m_ParticleShader.Use();

	for (Particle effects : system.m_Particles)
	{
		this->m_ParticleShader.SetVector2f("offset", effects.transform.position);
		this->m_ParticleShader.SetVector4f("color", effects.material);
		system.m_Image.Bind();
		glBindVertexArray(system.VAO);
	}


	glDisable(GL_BLEND);
}

void Vertex2D::DrawLine(glm::vec2 _start, glm::vec2 _end, Material& _mat)
{
	float Lines[] =
	{
		_start.x, _start.y,  _end.x, _end.y,
	};

	glVertexPointer(2, GL_FLOAT, 0, Lines);
	glDrawArrays(GL_LINES, 0, 2);
}

void Vertex2D::SetUpData()
{
	unsigned int VBO;

	m_Vertices.push_back(glm::vec2(0.0f, 1.0f));
	m_Vertices.push_back(glm::vec2(0.0f, 1.0f));

	m_Vertices.push_back(glm::vec2(1.0f, 0.0f));
	m_Vertices.push_back(glm::vec2(1.0f, 0.0f));

	m_Vertices.push_back(glm::vec2(0.0f, 0.0f));
	m_Vertices.push_back(glm::vec2(0.0f, 0.0f));

	m_Vertices.push_back(glm::vec2(0.0f, 1.0f));
	m_Vertices.push_back(glm::vec2(0.0f, 1.0f));

	m_Vertices.push_back(glm::vec2(1.0f, 1.0f));
	m_Vertices.push_back(glm::vec2(1.0f, 1.0f));

	m_Vertices.push_back(glm::vec2(1.0f, 0.0f));
	m_Vertices.push_back(glm::vec2(1.0f, 0.0f));

	float vertices[24];
	int m_CurrentVertice = 0;
	for (int i = 0; i < 24; i += 2)
	{
		vertices[i] = m_Vertices.at(m_CurrentVertice).x;
		vertices[i + 1] = m_Vertices.at(m_CurrentVertice).y;
		m_CurrentVertice++;
	}

	glGenVertexArrays(1, &this->m_quadVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(this->m_quadVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}