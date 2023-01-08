#include "GameObject.h"

GameObject::GameObject()
{
	name = "GameObject";
	m_Active = true;

	m_Collider = new Collider();
	transform.localPosition.y = 0;
	transform.localPosition.x = 0;

	material = Material("Mat");
	transform.size.x = 1;
	transform.size.y = -1;
}

GameObject::GameObject(const char* _Name, bool active)
{
	name = _Name;
	m_Active = active;
	transform.localPosition.y = 0;
	transform.localPosition.x = 0;

	material = Material("Mat");
	transform.size.x = 1;
	transform.size.y = -1;
	 
	m_Collider = new Collider();
}

GameObject::GameObject(const char* _Name)
{
	name = _Name;
	m_Active = true;
	transform.localPosition.y = 0;
	transform.localPosition.x = 0;

	material = Material("Mat");
	m_Collider = new Collider();
	transform.size.x = 1;
	transform.size.y = -1;
}

void GameObject::SetParent(GameObject* _object)
{
	if (m_Parent != nullptr)
	{
		m_Parent->m_Children.remove(this);
	}

	m_Parent = _object;

	if (m_Parent != nullptr)
	{
		m_Parent->m_Children.push_back(this);
	}
}

void GameObject::RemoveParent()
{
	if (m_Parent != nullptr)
	{
		m_Parent->m_Children.remove(this);
	}
}

void GameObject::SetChild(GameObject* _child)
{
	m_Children.push_back(_child);
}

void GameObject::RemoveChild()
{
	//TODO: Add children remove
}

void GameObject::ConfigureSystems()
{
	if (m_Parent != nullptr)
	{
		transform.position = m_Parent->transform.position + transform.localPosition;
	}
}

void GameObject::AddComponent(VertexComponent* _comp)
{
	m_Components.push_back(_comp);
}

bool GameObject::GetComponent(VertexComponent& _target)
{
	for (int i = 0; i < m_Components.size(); i++) {
		if (typeid(_target) == typeid(m_Components.at(i))) 
		{
			_target = *m_Components.at(i);
			return true;
		}
	}

	return false;
}

void GameObject::RemoveComponent(VertexComponent* _comp)
{
	for (int i = 0; i < m_Components.size(); i++)
	{
		if (_comp == m_Components.at(i))
		{
			m_Components.erase(m_Components.begin() + i);
		}
	}
}
