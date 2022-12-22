#pragma once
#include "Transform.h"
#include <string>
#include "Texture2D.h"
#include <list>
#include "Material.h"
enum Collider {
	NONE = 0,
	AABB,
	CIRCLE,
};

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

	void SetChild(GameObject* _child);
	void RemoveChild();
	void ConfigureSystems();
	void SetCollider(Collider _type) { m_ColliderType = _type; }
	Collider GetCollider() { return m_ColliderType; }

private:
	Collider m_ColliderType = NONE;
	GameObject* m_Parent;
	std::list<GameObject*> m_Children;
};

