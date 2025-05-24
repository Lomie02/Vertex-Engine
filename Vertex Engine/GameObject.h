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
	~GameObject();

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


	template<typename T>
	std::vector<T*> FindComponentsOfType() {

		std::vector<T*> comps;
		for (auto temp : m_Componenets) {
			if (T* cast = dynamic_cast<T*>(temp))
			{
				comps.push_back(cast);

			}
		}
		return comps;
	}

	template<typename T, typename... Args>
	T* AddComponent(Args&&... args) {
		T* componenet = new T(std::forward<Args>(args)...);
		componenet->partner2d = this;
		m_Componenets.push_back(componenet);
		return componenet;
	}

	/// <summary>
	/// Add a Componenet by type
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <returns></returns>
	template<typename T>
	T* AddComponent() {
		T* componenet = new T();
		componenet->partner2d = this;
		m_Componenets.push_back(componenet);
		return componenet;
	}

	/// <summary>
	/// Gets the first component of type.
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <returns></returns>
	template<typename T>
	T* GetComponenet() {
		if (m_Componenets.empty) return;
		for (auto& comp : m_Componenets) {
			if (T* object = dynamic_cast<T*>(comp)) {
				return object;
			}
		}
		return nullptr;
	}

	/// <summary>
	/// Remove Component of Type
	/// </summary>
	/// <typeparam name="T"></typeparam>
	template<typename T>
	void RemoveComponenet() {
		for (auto i = m_Componenets.begin(); i != m_Componenets.end(); i++) {
			if (T* cast = dynamic_cast<T*>(*i)) {
				delete* i;
				*i = nullptr;
				m_Componenets.erase(i);
				return;
			}
		}
	}

	/// <summary>
	/// Remove a given component.
	/// </summary>
	/// <param name="comp"></param>
	void RemoveComponent(VertexComponent* comp) {
		auto i = std::find(m_Componenets.begin(), m_Componenets.end(), comp);
		if (i != m_Componenets.end()) {
			delete* i;
			*i = nullptr;
			m_Componenets.erase(i);
		}
	}

	std::vector<VertexComponent*> GetEntireComponenetList() { return m_Componenets; }

	class Vertex2D;
private:

	std::vector<VertexComponent*> m_Componenets;

	std::vector<MimeProfile> m_Mimes = std::vector<MimeProfile>(10);
	int m_MimesAmount = 0;
	void MaterialConfigure();
	void ConfigurePartners();
	glm::vec2 m_Velocity;

	Collider* m_Collider;
	GameObject* m_Parent;
	std::vector<GameObject*> m_Children;
};

