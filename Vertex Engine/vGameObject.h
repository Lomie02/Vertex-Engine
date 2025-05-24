#pragma once
#include "vTransform.h"
#include <string>
#include <vector>
#include "VertexComponent.h"
#include <iostream>
class vGameObject
{
public:
	vGameObject();
	vGameObject(std::string _Name);
	vGameObject(std::string _Name, bool active);
	~vGameObject();

	vTransform transform;

	void SetTag(std::string _tag) { m_Tag = _tag; }
	std::string GetTag() { return m_Tag; }

	void SetName(std::string _name) { m_ObjectName = _name; }
	std::string GetName() { return m_ObjectName; }

	void SetActive(bool _state) { m_IsActive = _state; }
	bool GetActive() { return m_IsActive; }

	// Componenet Commands

	template<typename T>
	std::vector<T*> FindComponentsOfType() {

		std::vector<T*> comps;
		for (auto temp : m_Componenets) {
			if (T* cast = dynamic_cast<T*>(temp))
				comps.push_back(cast);
		}
		return comps;
	}

	/// <summary>
	/// Add already existing componenets.
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <typeparam name="...Args"></typeparam>
	/// <param name="...args"></param>
	/// <returns></returns>
	template<typename T, typename... Args>
	T* AddComponent(Args&&... args) {
		T* componenet = new T(std::forward<Args>(args)...);
		componenet->partner3d = this;
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
		componenet->partner3d = this;
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
		for (auto i = m_Componenets.begin(); i = m_Componenets.end(); i++) {
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
			delete *i;
			*i = nullptr;
			m_Componenets.erase(i);
		}
	}

	std::vector<VertexComponent*> GetEntireComponenetsList() { return m_Componenets; }

protected:

	std::vector<VertexComponent*> m_Componenets;

	vGameObject* m_Parent;
	std::vector<vGameObject*> m_Children;
	std::string m_ObjectName;
	std::string m_Tag;
	bool m_IsActive;
};

