#include "GameObject.h"
#include "GameUniqueIdentityAsset.h"
#include "Random.h"
#include "gtx/matrix_decompose.hpp"
#include "RectTransform.h"

#define GLM_ENABLE_EXPERIMENTAL
GameObject::GameObject()
{
	GenerateIds();
	name = "GameObject";
	m_Active = true;
	m_Collider = new Collider();

	AddComponent<Transform>();
	transform = GetComponenet<Transform>();

	material = Material("Mat");
	AddComponent<Material>(material);

	transform->scale.x = 5;
	transform->scale.y = 5;
	transform->scale.z = 5;

}

GameObject::GameObject(const char* _Name, bool active)
{
	GenerateIds();
	name = _Name;
	m_Active = active;
	AddComponent<Transform>();
	transform = GetComponenet<Transform>();

	material = Material("Mat");
	AddComponent<Material>(material);

	transform->scale.x = 5;
	transform->scale.y = 5;
	transform->scale.z = 5;

	m_Collider = new Collider();
}

GameObject::~GameObject()
{
	for (auto& comps : m_Componenets) {

		delete comps;
		comps = nullptr;
	}
	m_Componenets.clear();
}

glm::vec4 GameObject::ColourIDConversion(uint16_t _id)
{
	return glm::vec4(
		(uint8_t)(_id & 0xFF) / 255.0f,
		(uint8_t)((_id >> 8) & 0xFF) / 225.0f,
		(uint8_t)((_id >> 16) & 0xFF) / 255.0f,
		1.0f
	);
}

uint32_t GameObject::IdColourConversion(const glm::vec4& _col)
{
	uint8_t r = static_cast<uint8_t>(_col.r * 255.0f);
	uint8_t g = static_cast<uint8_t>(_col.g * 255.0f);
	uint8_t b = static_cast<uint8_t>(_col.b * 255.0f);

	return (r) | (g << 8) | (b << 16);
}

GameObject::GameObject(const char* _Name)
{
	GenerateIds();
	name = _Name;
	m_Active = true;
	AddComponent<Transform>();
	transform = GetComponenet<Transform>();


	material = Material("Mat");
	m_Collider = new Collider();
	transform->scale.x = 5;
	transform->scale.y = 5;
	transform->scale.z = 5;

}

void GameObject::ConfigureSystems()
{
	MaterialConfigure();
	transform->SetLayer(layer);
	/*if (m_Parent) {
		glm::mat4 ParentTransform = glm::translate(m_Parent->transform.rotation, m_Parent->transform.position);
	}*/

	/*if (m_Parent != nullptr)
	{
		transform.position = m_Parent->transform.position + transform.localPosition;
		transform.rotation = m_Parent->transform.rotation + transform.localRotation;
		transform.scale = m_Parent->transform.scale / transform.localScale;
	}*/
}

void GameObject::InstanceMime(std::string _name, glm::vec3 _pos)
{
	m_Mimes.at(m_MimesAmount).Name = _name;
	m_Mimes.at(m_MimesAmount).transform.position = _pos;
	m_MimesAmount++;
}

MimeProfile GameObject::InstanceEditMime(std::string _name)
{
	for (int i = 0; i < m_Mimes.size(); i++) {
		if (m_Mimes.at(i).Name == _name)
		{
			return m_Mimes.at(i);
		}
	}

	return MimeProfile();
}

void GameObject::GenerateIds()
{
	m_ObjectUniqueIdentitfier = GameUniqueIdentityAsset::GenerateUniqueIdenityIndex();
	colourPickerColour = ColourIDConversion(m_ObjectUniqueIdentitfier);
}

void GameObject::MaterialConfigure()
{
	if (material.m_KeepAspect)
	{
		transform->scale.x = material.AlbedoMap.Width;
		transform->scale.y = material.AlbedoMap.Height;
	}
}

void GameObject::ConfigurePartners()
{
}
