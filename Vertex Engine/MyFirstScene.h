#pragma once
#include "VertexScene.h"
#include "Camera.h"
#include "GameObject.h"

class MyFirstScene : public VertexScene
{
public:
	MyFirstScene();
	MyFirstScene(const char _name[]);
	~MyFirstScene();
	virtual void Awake();
	virtual void Start();
	virtual void Update(float delta);
	virtual void LateUpdate(float delta);
	virtual void FixedUpdate(float fixedDelta);

	AssetManager* GetAssets() { return m_Manager; }

private:

	GameObject* m_GameObject;
	Camera* m_MyCamera;

};

