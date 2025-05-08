#include "vGameObject.h"

vGameObject::vGameObject()
{
	SetName("Empty GameObject");
	SetTag("Untagged");
}

vGameObject::vGameObject(std::string _Name)
{
	SetName(_Name);
	SetTag("Untagged");
}

vGameObject::vGameObject(std::string _Name, bool active)
{
	SetName(_Name);
	SetTag("Untagged");
	SetActive(active);
}

vGameObject::~vGameObject()
{
	for (auto& comp : m_Componenets) {
		delete comp;
		comp = nullptr;
	}
}

