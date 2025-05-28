#pragma once
#include "VertexScene.h"
#include "SceneManager.h"
#include "Vertex2D.h"
#include "Texture2D.h"

#include "AssetManager.h"
#include "VertexGeneral.h"
#include "bullet/btBulletDynamicsCommon.h"

#include "DebugComp.h"
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
	AssetManager* GetAssets() { return m_Manager; }

	void StartFlipbookSetUp();


private:

	void SetupButton();
	SceneManager* m_SceneManager;
	GLFWwindow* m_Window;

	RigidBody* m_Body;
	RigidBody* m_Decoy;
	RigidBody* m_BlockBody;

	Camera* m_CamMain;
	Camera* m_CamRender;

	// Camaeras
	GameObject* m_Camera01;
	GameObject* m_Camera02;
	// GameObjects
	GameObject* m_Object2;
	GameObject* m_Object0;
	GameObject* m_Block;
	GameObject* m_Egg;

	DebugComp* m_Componenet;
	vGameObject* m_GameObject;
	
	//User Interface
	Text* m_Title;
	Text* m_CounterText;
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

	RenderTexture* m_TexureRender;
};

