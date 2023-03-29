#pragma once
#include "VertexScene.h"
#include "SceneManager.h"
#include "Vertex2D.h"
#include "Texture2D.h"

#include "GameObject.h"
#include "AssetManager.h"
#include "Camera.h"

class Scene2 : public VertexScene
{
public:
	Scene2();
	Scene2(const char _name[]);
	~Scene2();

	virtual void Awake();
	virtual void Start();
	virtual void Update(float delta);
	virtual void LateUpdate(float delta);
	virtual void FixedUpdate(float fixedDelta);
	virtual void Rendering(Vertex2D* renderer);

	AssetManager GetAssets() { return m_Manager; }

private:

	GameObject* m_Object2;
	GameObject* m_Object0;

	Camera* m_MainCamera;
};

