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

#define GRAPH_X 40
#define GRAPH_Y 40

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

	void GiveWindow(GLFWwindow* window) { m_Window = window; }
	AssetManager* GetAssets() { return m_Manager; }
private:

	void AssignTextures();
	TestComponent* m_MyComponent;
	SceneManager* m_SceneManager;
	GLFWwindow* m_Window;

	Texture2D Dont;

	AudioSource* m_SoundEffect;
	RigidBody* m_Body;
	Volume m_MyVolume;

	Text* m_MyText;
	Button* m_Button;

	GameObject* m_LadysFace;
	GameObject* m_MansFace;
	GameObject* m_Static;

	FlipClip m_Clip;
	Flipbook* m_StaticAnimation;
	float counter;
	Camera* m_MainCamera;
	Camera* m_InterfaceCamera;
};