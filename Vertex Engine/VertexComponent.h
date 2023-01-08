#pragma once

#include "GameObject.h"

class VertexComponent
{
public:
	VertexComponent();

	virtual void Start();
	virtual void Update(float delta);

	virtual void FixedUpdate(float delta);
	virtual void LateUpdate(float delta);

	void AssignPartner(GameObject* _partner);
	void RemovePartner();

protected:

	GameObject* Partner;

};

