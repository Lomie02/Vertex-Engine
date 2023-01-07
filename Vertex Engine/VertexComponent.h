#pragma once
class VertexComponent
{
public:
	VertexComponent();

	virtual void Start();
	virtual void Update(float delta);

	virtual void FixedUpdate(float delta);
	virtual void LateUpdate(float delta);

protected:

};

