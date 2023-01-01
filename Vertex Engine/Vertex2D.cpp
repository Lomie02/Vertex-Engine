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
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	this->m_Shader = material.shader;

	this->m_Shader.Use();
	glm::mat4 model = glm::mat4(1.0f);

	model = glm::translate(model, glm::vec3(position, 0.0f));
	model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * -size.y, 0.0f));
	model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * -size.y, 0.0f));

	model = glm::scale(model, glm::vec3(size.x * scale ,-size.y * scale, 1.0f));
	this->m_Shader.SetMatrix4("model", model);
	this->m_Shader.SetMatrix4("pro", per);
	this->m_Shader.SetVector3f("spriteColor", material.colour);
	glActiveTexture(GL_TEXTURE0);
	material.baseTexture.Bind();

	glBindVertexArray(this->m_quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	glDisable(GL_BLEND);
}

void Vertex2D::DrawLine(glm::vec2 _start, glm::vec2 _end, Material& _mat)
{

	_start.x = 2 * _start.x / 1920 - 1;
	_start.y = 2 * _start.y / 1080 - 1;

	_end.x = 2 * _end.x / 1920 - 1;
	_end.y = 2 * _end.y / 1080 - 1;

	glBegin(GL_LINES);
	glColor3f(_mat.colour.r,_mat.colour.g, _mat.colour.b);
	glVertex2f(_start.x, _start.y);
	glVertex2f(_end.x, _end.y);
	glEnd();
}

void Vertex2D::SetUpData()
{
	unsigned int VBO;
	float vertices[] = {
		// pos      // tex
	0.0f, 1.0f,		 0.0f, 1.0f,
	1.0f, 0.0f,		 1.0f, 0.0f,
	0.0f, 0.0f,		 0.0f, 0.0f,

	0.0f, 1.0f,		 0.0f, 1.0f,
	1.0f, 1.0f,	     1.0f, 1.0f,
	1.0f, 0.0f,		 1.0f, 0.0f
	};

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