#pragma once
#include "Material.h"
#include "Transform.h"
class Sprite // Sprites are used for UI based images. Not to be confused with gameobjects.
{
public:
	Sprite();
	Sprite(const char* _name);
	~Sprite();

	void SetActive(bool _state) { m_IsActive = _state; }
	bool GetActive() { return m_IsActive; }

	Transform transform;
	Material material;
private:
	bool m_IsActive = true;
	const char* m_name = "Sprite";
};

