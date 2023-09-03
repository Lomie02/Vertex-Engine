#pragma once
#include "Transform.h"
#include <string>
#include "Texture2D.h"
#include <list>
#include "Material.h"
#include "Collider.h"
#include "VertexComponent.h"
#include <iostream>

#include "Mime.h"

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

	std::string Tag = "Untagged";
	int layer;

	void InstanceMime(std::string _name, glm::vec2 _pos);
	MimeProfile InstanceEditMime(std::string _name);
	std::vector<MimeProfile> GetMimes() { return m_Mimes; }

	int GetActiveMimesSize() { return m_MimesAmount; }
	glm::vec2 GetVelocity() { return m_Velocity; }
	void SetVelocity(glm::vec2 _vel) { m_Velocity += _vel; }

	class Vertex2D;
private:

	std::vector<MimeProfile> m_Mimes = std::vector<MimeProfile>(10);
	int m_MimesAmount = 0;
	void MaterialConfigure();
	void ConfigurePartners();
	glm::vec2 m_Velocity;

	Collider* m_Collider;
	GameObject* m_Parent;
	std::vector<GameObject*> m_Children;
};

