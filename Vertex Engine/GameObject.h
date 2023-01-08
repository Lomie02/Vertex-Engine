#pragma once
#include "Transform.h"
#include <string>
#include "Texture2D.h"
#include <list>
#include "Material.h"
#include "Collider.h"
#include "VertexComponent.h"
#include <iostream>
class GameObject
{
public:
	GameObject();
	GameObject(const char* _Name);
	GameObject(const char* _Name, bool active);
	void SetActive(bool _active) { m_Active = _active; }
	bool GetActive() { return m_Active; }

	Material material;
	Transform transform;

	const char* name = "GameObject";
	bool m_Active;

	void SetParent(GameObject* _object);
	void RemoveParent();

	GameObject* GetParent() { return m_Parent; }
	void SetChild(GameObject* _child);
	void RemoveChild();
	void ConfigureSystems();
	Collider* GetCollider() { return m_Collider; }

	void AddComponent(VertexComponent* _comp);

	bool GetComponent(VertexComponent& _target);
	void RemoveComponent(VertexComponent* _comp);

	int ComponentCount() { return m_Components.size(); }
	std::vector<VertexComponent*> ComponentList() { return m_Components; }

	std::string Tag = "Untagged";
private:
	std::vector<VertexComponent*> m_Components;
	Collider* m_Collider;
	GameObject* m_Parent;
	std::list<GameObject*> m_Children;
};

