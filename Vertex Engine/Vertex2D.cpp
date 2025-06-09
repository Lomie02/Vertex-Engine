#include "Vertex2D.h"
#include "RectTransform.h"
#include "SpriteRenderer.h"
Vertex2D::Vertex2D(Shader& shader)
{
	DefaultSpriteMat = new Material("VertexDefaultSpriteMaterial");

	this->m_Shader = shader;
	this->SetUpData();
}

Vertex2D::~Vertex2D()
{
	delete DefaultSpriteMat;
	DefaultSpriteMat = nullptr;

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
	material.AlbedoMap.Bind();

	glBindVertexArray(this->m_quadVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glDisable(GL_CULL_FACE);

	CompletedDrawCall();
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

	if (_object->transform->GetParent() != nullptr)
	{
		model = glm::translate(model, glm::vec3(_object->transform->GetParent()->position.x, _object->transform->GetParent()->position.y, 0.0f));
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
	material.AlbedoMap.Bind();

	glBindVertexArray(this->m_quadVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	//glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glDisable(GL_CULL_FACE);
	CompletedDrawCall();
}

void Vertex2D::TensionDraw(GameObject* _object, glm::mat4 per)
{
	//glEnable(GL_CULL_FACE);

	bool m_UseSpriteRendererData = false;

	// See if a gameobject has a Sprite Renderer component attached. If it does, render with the comp data instead of default materials. 
	if (_object->GetComponenet<SpriteRenderer>())
		m_UseSpriteRendererData = true;

	switch (m_UseSpriteRendererData) {
	case true:

		if (_object->GetComponenet<SpriteRenderer>()->Colour.w < 1); // auto detect if transparent
		{
			glEnable(GL_BLEND);

			switch (_object->material.TransparencyBlend) {
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
		this->m_Shader = DefaultSpriteMat->shader;

		this->m_Shader.Use();

		if (RENDER_DEPTH_TEST == false) {
			this->m_Shader.SetInteger("UseDepth", 0);
		}
		else {

			this->m_Shader.SetInteger("UseDepth", 1);
		}


		this->m_Shader.SetMatrix4("model", _object->transform->GetWorldModelMat());
		this->m_Shader.SetMatrix4("pro", per);
		this->m_Shader.SetInteger("picking", 0);
		this->m_Shader.SetInteger("NoTexture", 0);
		this->m_Shader.SetVector4f("Colour", _object->GetComponenet<SpriteRenderer>()->Colour);

		glActiveTexture(GL_TEXTURE0);
		DefaultSpriteMat->AlbedoMap = _object->GetComponenet<SpriteRenderer>()->Sprite;

		DefaultSpriteMat->AlbedoMap.Bind();

		break;

	case false: // Normal rendering without sprite comp

		if (_object->material.surface == Transparent)
		{
			glEnable(GL_BLEND);

			switch (_object->material.TransparencyBlend) {
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
		this->m_Shader = _object->material.shader;

		this->m_Shader.Use();

		if (RENDER_DEPTH_TEST == false) {
			this->m_Shader.SetInteger("UseDepth", 0);
		}
		else {

			this->m_Shader.SetInteger("UseDepth", 1);
		}


		this->m_Shader.SetMatrix4("model", _object->transform->GetWorldModelMat());
		this->m_Shader.SetMatrix4("pro", per);
		this->m_Shader.SetInteger("picking", 0);
		this->m_Shader.SetInteger("NoTexture", 0);
		this->m_Shader.SetVector4f("Colour", _object->material.colour);

		glActiveTexture(GL_TEXTURE0);
		_object->material.AlbedoMap.Bind();
		break;
	}
	

	PrepareRender();

	if (USE_DEPTH_TESTING) {
		glDisable(GL_DEPTH_TEST);
	}
	glDisable(GL_BLEND);
	glDisable(GL_CULL_FACE);
	CompletedDrawCall();
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

		this->m_Shader.SetMatrix4("model", _list.at(i)->transform->GetWorldModelMat());
		this->m_Shader.SetMatrix4("pro", per);
		this->m_Shader.SetVector4f("Colour", _list.at(i)->material.colour);
		this->m_Shader.SetInteger("NoTexture", 0);

		glActiveTexture(GL_TEXTURE0);
		_list.at(i)->material.AlbedoMap.Bind();

		glBindVertexArray(this->m_quadVAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
		glDisable(GL_CULL_FACE);
		CompletedDrawCall();
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

	ImageOffset.x = _sprite->transform.scale.x / 2;
	ImageOffset.y = _sprite->transform.scale.y / 2;

	//TODO: Offset causes sprites position to change.

	//_sprite->transform.position.x -= ImageOffset.x;
	//_sprite->transform.position.y += ImageOffset.y;

	glm::vec2 GeneralPivot;
	Transform ParentsTransform;

	GeneralPivot = _sprite->transform.position + _sprite->transform.pivot;

	//========================================== Main Render calulations for transforms.

	this->m_Shader.SetVector3f("lights", glm::vec3(1.0, 1.0, 1.0));
	this->m_Shader.SetInteger("NoTexture", 0);

	this->m_Shader.SetMatrix4("pro", _pro);
	this->m_Shader.SetVector4f("Colour", _sprite->material.colour);

	glActiveTexture(GL_TEXTURE0);
	_sprite->material.AlbedoMap.Bind();

	PrepareRender();

	if (USE_DEPTH_TESTING) {
		glDisable(GL_DEPTH_TEST);
	}
	glDisable(GL_BLEND);
	glDisable(GL_CULL_FACE);
	CompletedDrawCall();
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

void Vertex2D::TensionInterfaceDraw(GameObject* _element, bool _IsColourPick) //TODO: Allow the renderer to examine for UI based Componenets
{
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);

	this->m_Shader = _element->material.shader;
	uint32_t id = _element->GetUniqueIdentity();
	glm::vec4 colour = _element->GetColourPickerCol();

	this->m_Shader.Use();

	//=======================================

	this->m_Shader.SetMatrix4("model", _element->GetComponenet<RectTransform>()->GetWorldMatrix());
	this->m_Shader.SetInteger("picking", 0);
	this->m_Shader.SetInteger("NoTexture", 1);

	this->m_Shader.SetMatrix4("pro", glm::ortho(0.0f, static_cast<float>(1920), static_cast<float>(1080), 0.0f, -0.100f, 10.0f));
	this->m_Shader.SetVector4f("idColour", colour);
	this->m_Shader.SetVector4f("Colour", colour);

	glActiveTexture(GL_TEXTURE0);
	_element->material.AlbedoMap.Bind();

	PrepareRender();
}

void Vertex2D::VertexEngineColourPickRender(GameObject* _object, glm::mat4 per)
{
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);

	this->m_Shader = _object->material.shader;
	uint32_t id = _object->GetUniqueIdentity();
	glm::vec4 colour = _object->GetColourPickerCol();

	this->m_Shader.Use();

	//=======================================

	this->m_Shader.SetMatrix4("model", _object->transform->GetWorldModelMat());
	this->m_Shader.SetInteger("picking", 1);
	this->m_Shader.SetMatrix4("pro", per);
	this->m_Shader.SetVector4f("idColour", colour);
	this->m_Shader.SetVector4f("Colour", _object->material.colour);

	glActiveTexture(GL_TEXTURE0);
	_object->material.AlbedoMap.Bind();

	glBindVertexArray(this->m_quadVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

}

void Vertex2D::PrepareRender()
{
	glBindVertexArray(this->m_quadVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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
	unsigned int EBO;

	float vertices3D[] = {
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

	};

	unsigned int indices[] = {
		0,1,2,2,3,0
	};

	glGenVertexArrays(1, &this->m_quadVAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(m_quadVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices3D), vertices3D, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindVertexArray(this->m_quadVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}