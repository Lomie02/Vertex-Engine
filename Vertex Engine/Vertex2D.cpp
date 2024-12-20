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
	//glEnable(GL_BLEND);
	glEnable(GL_CULL_FACE);

	//glEnable(GL_DEPTH_TEST);
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

	//glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glDisable(GL_CULL_FACE);
}

void Vertex2D::TensionDraw(GameObject* _object, Material& material, glm::vec2 position, glm::vec2 size, float rotate, float scale, glm::mat4 per, int _RenderLayer)
{
	//glEnable(GL_CULL_FACE);

	if (material.surface == Transparent)
	{
		glEnable(GL_BLEND);

		switch (material.TransparencyBlend) {
		case Alpha:
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			break;
		case Additive:
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			break;

		case Screen:
			glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
			break;
		}
	}
	if (USE_DEPTH_TESTING) {
		glEnable(GL_DEPTH_TEST);
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

	// Vertex Filters.
	if (m_VertexVolume.ChromaticAberation.ChromaticEnabled) {
		this->m_Shader.SetInteger("UseChromatic", 1);
		this->m_Shader.SetFloat("ChromaticOffset", m_VertexVolume.ChromaticAberation.ChromaticIntensity);
	}
	if (m_VertexVolume.Invert.InvertedEnabled) {
		this->m_Shader.SetFloat("UseInvert", 1);
	}

	//=======================================
	//glm::mat4 model = glm::mat4(1.0f);

	glm::mat4 model = glm::mat4(1);

	// Pivots & Transform calulations,=.
	glm::vec2 ImageOffset;

	ImageOffset.x = _object->transform.size.x / 2;
	ImageOffset.y = _object->transform.size.y / 2;

	position.x -= ImageOffset.x;
	position.y += ImageOffset.y;

	glm::vec2 GeneralPivot;
	Transform ParentsTransform;

	if (_object->GetParent() != nullptr) { // If there is a parent then combine the 
		ParentsTransform = _object->GetParent()->transform;

		GeneralPivot = ParentsTransform.position + _object->transform.localPosition;
	}
	else {// If no parent then use objects pivot & position.
		GeneralPivot = position + _object->transform.pivot;
	}

	//========================================== Main Render calulations for transforms.

	model = glm::translate(model, glm::vec3(GeneralPivot, (float)_RenderLayer)); // position

	model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * -size.y, 0.0f)); // Position
	model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotation
	model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * -size.y, 0.0f));

	model = glm::scale(model, glm::vec3(size.x * scale, -size.y * scale, 1.0f));
	this->m_Shader.SetVector3f("lights", glm::vec3(1.0, 1.0, 1.0));

	this->m_Shader.SetMatrix4("model", model);
	this->m_Shader.SetMatrix4("pro", per);
	this->m_Shader.SetVector4f("Colour", material.colour);

	glActiveTexture(GL_TEXTURE0);
	material.baseTexture.Bind();

	glBindVertexArray(this->m_quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	if (USE_DEPTH_TESTING) {
		glDisable(GL_DEPTH_TEST);
	}
	glDisable(GL_BLEND);
	glDisable(GL_CULL_FACE);
}

void Vertex2D::TensionParticle(ParticleSystem& system)
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

void Vertex2D::TensionTransparencyPass(std::vector<GameObject*> _list, glm::mat4 per)
{
	for (int i = _list.size(); i > 0; i--) {

		glm::mat4 model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(_list.at(i)->transform.position, _list.at(i)->layer));

		model = glm::translate(model, glm::vec3(0.5f * _list.at(i)->transform.size.x, 0.5f * -_list.at(i)->transform.size.y, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(-0.5f * _list.at(i)->transform.size.x, -0.5f * -_list.at(i)->transform.size.y, 0.0f));

		model = glm::scale(model, glm::vec3(_list.at(i)->transform.size.x * _list.at(i)->transform.scale, -_list.at(i)->transform.size.y * _list.at(i)->transform.scale, 1.0f));
		this->m_Shader.SetMatrix4("model", model);
		this->m_Shader.SetMatrix4("pro", per);
		this->m_Shader.SetVector4f("Colour", _list.at(i)->material.colour);

		glActiveTexture(GL_TEXTURE0);
		_list.at(i)->material.baseTexture.Bind();

		glBindVertexArray(this->m_quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
		glDisable(GL_CULL_FACE);
	}
}

void Vertex2D::TensionSprite(Sprite* _sprite, glm::mat4 _pro) //TODO May redo this part of the renderer to base positions from the canvas instead.
{
	if (_sprite->material.surface == Transparent)
	{
		glEnable(GL_BLEND);

		switch (_sprite->material.TransparencyBlend) {
		case Alpha:
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			break;
		case Additive:
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			break;

		case Screen:
			glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
			break;
		}
	}
	if (USE_DEPTH_TESTING) {
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
	}
	this->m_Shader = _sprite->material.shader;

	this->m_Shader.Use();

	if (RENDER_DEPTH_TEST == false) {
		this->m_Shader.SetInteger("UseDepth", 0);
	}
	else {

		this->m_Shader.SetInteger("UseDepth", 1);
	}

	// Vertex Filters.
	if (m_VertexVolume.ChromaticAberation.ChromaticEnabled) {
		this->m_Shader.SetInteger("UseChromatic", 1);
		this->m_Shader.SetFloat("ChromaticOffset", m_VertexVolume.ChromaticAberation.ChromaticIntensity);
	}
	if (m_VertexVolume.Invert.InvertedEnabled) {
		this->m_Shader.SetFloat("UseInvert", 1);
	}

	//=======================================
	//glm::mat4 model = glm::mat4(1.0f);

	glm::mat4 model = glm::mat4(1);

	// Pivots & Transform calulations,=.
	glm::vec2 ImageOffset;

	ImageOffset.x = _sprite->transform.size.x / 2;
	ImageOffset.y = _sprite->transform.size.y / 2;

	//TODO: Offset causes sprites position to change.

	//_sprite->transform.position.x -= ImageOffset.x;
	//_sprite->transform.position.y += ImageOffset.y;

	glm::vec2 GeneralPivot;
	Transform ParentsTransform;

	GeneralPivot = _sprite->transform.position + _sprite->transform.pivot;

	//========================================== Main Render calulations for transforms.

	model = glm::translate(model, glm::vec3(_sprite->transform.position, 0)); // position

	model = glm::translate(model, glm::vec3(0.5f * _sprite->transform.size.x, 0.5f * -_sprite->transform.size.y, 0.0f)); // Position
	model = glm::rotate(model, glm::radians(_sprite->transform.rotation), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotation
	model = glm::translate(model, glm::vec3(-0.5f * _sprite->transform.size.x, -0.5f * -_sprite->transform.size.y, 0.0f));

	model = glm::scale(model, glm::vec3(_sprite->transform.size.x * _sprite->transform.scale, -_sprite->transform.size.y * _sprite->transform.scale, 1.0f));
	this->m_Shader.SetVector3f("lights", glm::vec3(1.0, 1.0, 1.0));

	this->m_Shader.SetMatrix4("model", model);
	this->m_Shader.SetMatrix4("pro", _pro);
	this->m_Shader.SetVector4f("Colour", _sprite->material.colour);

	glActiveTexture(GL_TEXTURE0);
	_sprite->material.baseTexture.Bind();

	glBindVertexArray(this->m_quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	if (USE_DEPTH_TESTING) {
		glDisable(GL_DEPTH_TEST);
	}
	glDisable(GL_BLEND);
	glDisable(GL_CULL_FACE);

}

void Vertex2D::Tension_Bind_FrameBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Vertex2D::Tension_unBind_FrameBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Vertex2D::Tension_Rescale_FrameBuffer(float width, float height)
{
}

void Vertex2D::TensionVolume(Volume& _vol)
{
	m_VertexVolume = _vol;
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