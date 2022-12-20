#pragma once
#include "VertexScene.h"
#include "SceneManager.h"
#include "Vertex2D.h"
#include "Texture2D.h"

#include "GameObject.h"
#include "AssetManager.h"
#include "Camera.h"
#include "Button.h"

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

	SceneManager* m_SceneManager;
	GLFWwindow* m_Window;

	Texture2D Dont;
	Texture2D Dont2;

	Button* m_Button;
	VertexText2D* m_TextRenderer;
	GameObject* m_Object;
	GameObject* m_Object2;

	float counter;
	Camera* m_MainCamera;
};

