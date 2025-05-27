#include "GameObject.h"

GameObject::GameObject()
{
	name = "GameObject";
	m_Active = true;

	m_Collider = new Collider();
	transform.localPosition.y = 0;
	transform.localPosition.x = 0;

	material = Material("Mat");
	AddComponent<Material>(material);
	transform.size.x = 5;
	transform.size.y = 5;

}

GameObject::GameObject(const char* _Name, bool active)
{
	name = _Name;
	m_Active = active;
	transform.localPosition.y = 0;
	transform.localPosition.x = 0;

	material = Material("Mat");
	AddComponent<Material>(material);

	transform.size.x = 5;
	transform.size.y = 5;

	m_Collider = new Collider();
}

GameObject::~GameObject()
{
	for (auto& comps : m_Componenets) {
		delete comps;
		comps = nullptr;
	}
}

GameObject::GameObject(const char* _Name)
{
	name = _Name;
	m_Active = true;
	transform.localPosition.y = 0;
	transform.localPosition.x = 0;


	material = Material("Mat");
	m_Collider = new Collider();
	transform.size.x = 5;
	transform.size.y = 5;
}

void GameObject::SetParent(GameObject* _object)
{
	if (m_Parent == _object) return;

	if (m_Parent)
	{
		auto& sib = m_Parent->m_Children;
		sib.erase(std::remove(sib.begin(), sib.end(), this), sib.end());
	}

	m_Parent = _object;

	if (_object)
	{
		m_Parent->m_Children.push_back(this);
	}
}

void GameObject::RemoveParent()
{
	if (m_Parent)
	{
		auto& sib = m_Parent->m_Children;
		sib.erase(std::remove(sib.begin(), sib.end(), this), sib.end());
	}
}

void GameObject::SetChild(GameObject* _child)
{
	if (_child) {
		_child->SetParent(this);
	}
}

void GameObject::RemoveChild(GameObject* _child)
{
	if (_child && _child->m_Parent == this) {
		_child->SetParent(nullptr);
	}
}

void GameObject::ConfigureSystems()
{
	MaterialConfigure();
	transform.SetLayer(layer);
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

void GameObject::InstanceMime(std::string _name, glm::vec2 _pos)
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

void GameObject::MaterialConfigure()
{
	if (material.m_KeepAspect)
	{
		transform.size.x = material.AlbedoMap.Width;
		transform.size.y = material.AlbedoMap.Height;
	}
}

void GameObject::ConfigurePartners()
{
}
