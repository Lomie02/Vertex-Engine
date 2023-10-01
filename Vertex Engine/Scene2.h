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

	void StartFlipbookSetUp();


private:

	void SetupButton();
	SceneManager* m_SceneManager;
	GLFWwindow* m_Window;

	// Camaeras
	Camera* m_MainCamera;
	Camera* m_UserCamera;
	Canvas* m_Canvas;
	// GameObjects
	GameObject* m_Object2;
	GameObject* m_Object0;
	GameObject* m_Block;
	GameObject* m_Egg;
	
	//User Interface
	Text* m_Title;
	Button* m_ButtonTest;
	Sprite* m_Sprite;

	// Vertex Componenets

	// Animation
	Flipbook* m_EggFlipBook;
	FlipClip m_IdleClip;
	FlipClip m_EggClip;

	Flipbook* m_FlipbookAnimation;
	Animator* m_Animation;
	
	//Controller
	PlayerController* m_Controller;

	//Timer

	float m_TitleTimer = 0;
	float m_Duration = 0.5f;
};

