#pragma once
#include "VertexScene.h"
#include "SceneManager.h"
#include "Vertex2D.h"
#include "Texture2D.h"

#include "AssetManager.h"
#include "VertexGeneral.h"

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

	void GiveWindow(GLFWwindow* window) { m_Window = window; }
	void GiveSceneManager(SceneManager* scenes) { m_SceneManager = scenes; }
	AssetManager GetAssets() { return m_Manager; }

private:

	Button* m_ButtonTest;
	GLFWwindow* m_Window;
	SceneManager* m_SceneManager;

	PlayerController m_PlayerCon;
	GameObject* m_Object2;
	GameObject* m_Object0;

	Camera* m_MainCamera;
};

