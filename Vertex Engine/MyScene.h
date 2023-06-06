#pragma once
#include "VertexScene.h"
#include "SceneManager.h"
#include "Vertex2D.h"
#include "Texture2D.h"

#include "AssetManager.h"
#include "Button.h"

#include "RigidBody.h"
#include "TestComponent.h"
#include "Text.h"

#include "VertexGeneral.h"

class MyScene : public VertexScene
{
public:
	MyScene();
	MyScene(const char _name[]);
	~MyScene();
	virtual void Awake();
	virtual void Start();
	virtual void Update(float delta);
	virtual void LateUpdate(float delta);
	virtual void FixedUpdate(float fixedDelta);
	virtual void Rendering(Vertex2D* renderer);

	void GiveWindow(GLFWwindow* window) { m_Window = window; }
	void GiveSceneManager(SceneManager* scenes) { m_SceneManager = scenes; }
	AssetManager GetAssets() { return m_Manager; }
private:

	TestComponent* m_MyComponent;
	SceneManager* m_SceneManager;
	GLFWwindow* m_Window;

	Texture2D Dont;
	Texture2D Dont2;

	RigidBody* m_Body;

	Text* m_MyText;

	Button* m_Button;
	GameObject* m_Object;
	GameObject* m_Object2;
	GameObject* m_Canvas;

	float counter;
	Camera* m_MainCamera;
	Camera* m_DummyCamera;
};