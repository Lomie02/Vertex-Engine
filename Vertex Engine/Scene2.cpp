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
}

void Scene2::Awake()
{
	m_Object2 = new GameObject("Object");

	m_Object2->texture = ResourceManager::GetTexture("boy1");
	m_Manager.Register(m_Object2);

	m_MainCamera = new Camera("Camera 1");

	m_MainCamera->transform.position.x = 0;
	m_MainCamera->transform.position.y = 0;
	m_Manager.Register(m_MainCamera);
}

void Scene2::Start()
{
	m_Manager.GiveWindow(m_Window);
}

void Scene2::Update(float delta)
{

}

void Scene2::LateUpdate(float delta)
{
	m_Manager.ConfigureSystems();
}

void Scene2::FixedUpdate(float fixedDelta)
{
}

void Scene2::Rendering(Vertex2D* render)
{
	m_Manager.ConfigureRenderSystems(render);
}
