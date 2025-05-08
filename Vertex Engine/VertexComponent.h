#pragma once

class vGameObject;
class GameObject;

class VertexComponent
{
public:
	virtual ~VertexComponent() = default;

	virtual void Start();
	virtual void Update(float delta);

	virtual void FixedUpdate(float delta);
	virtual void LateUpdate(float delta);

	vGameObject* partner3d;
	GameObject* partner2d;

};

