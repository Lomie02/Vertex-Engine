#include "Scene2.h"
#include <iostream>
#include "ResourceManager.h"
#include "SceneManager.h"

Scene2::Scene2() : VertexScene("DEFAULT_SCENE_VERTEX")
{
	Awake();
}

Scene2::Scene2(const char _name[]) : VertexScene(_name)
{
	Awake();
}

Scene2::~Scene2()
{
	delete m_Object2;
	m_Object2 = nullptr;

	delete m_MainCamera;
	m_MainCamera = nullptr;

	delete m_ButtonTest;
	m_ButtonTest = nullptr;
}

void Scene2::Awake()
{

	m_Object2 = new GameObject("Chimken");
	m_Object0 = new GameObject("Stupid ahh");

	m_Object0->material.m_KeepAspect = true;

	m_Object2->material.baseTexture = ResourceManager::LoadTexture("Builds/Textures/Chimken.png", "Chimken");
	m_Object2->material.baseTexture = ResourceManager::GetTexture("Chimken");

	m_Object2->material.surface = Transparent;
	m_Object0->material.surface = Transparent;


	m_Object0->transform.position.x = 3;
	m_Object0->layer = 1;
	m_Object2->layer = 3;

	m_Object0->material.baseTexture = ResourceManager::GetTexture("Chimken");
	m_Manager.Register(m_Object2);
	m_Manager.Register(m_Object0);

	m_MainCamera = new Camera("Camera 1");

	m_MainCamera->transform.position.x = 0;
	m_MainCamera->transform.position.y = 0;
	m_Manager.Register(m_MainCamera);

	m_ButtonTest = new Button("Goofy Button");
	m_Manager.Register(m_ButtonTest);

	SetupButton();
}

void Scene2::Start()
{
	glClearColor(0.0, 0.0, 0.1, 1.0);
	Cursor::Hide(m_Window);
	m_Manager.GiveWindow(m_Window);
}

void Scene2::Update(float delta)
{
	if (m_ButtonTest->Pressed())
	{

	}
}

void Scene2::LateUpdate(float delta)
{
}

void Scene2::FixedUpdate(float fixedDelta)
{
}

void Scene2::SetupButton()
{

	m_ButtonTest->transform.size.x = 10;
	m_ButtonTest->transform.size.y = 3;

	m_ButtonTest->transform.position.x = -5;
	m_ButtonTest->SetOffset(glm::vec2(-1, 0.3f));

	m_ButtonTest->text = "Vertex Ahh Button";

}

