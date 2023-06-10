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

	m_Object2->material.baseTexture = ResourceManager::GetTexture("boy1");
	m_Manager.Register(m_Object2);

	m_MainCamera = new Camera("Camera 1");

	m_PlayerCon;

	m_MainCamera->transform.position.x = 0;
	m_MainCamera->transform.position.y = 0;
	m_Manager.Register(m_MainCamera);

	m_PlayerCon.AssignPlayer(m_Object2);

}

void Scene2::Start()
{
}

void Scene2::Update(float delta)
{
	if (glfwGetKey(m_Window, GLFW_KEY_W) == GLFW_PRESS)
	{
		m_PlayerCon.MovePosition(glm::vec2(0, 1), delta);
	}

	if (glfwGetKey(m_Window, GLFW_KEY_S) == GLFW_PRESS)
	{
		m_PlayerCon.MovePosition(glm::vec2(0, -1), delta);
	}

	if (glfwGetKey(m_Window, GLFW_KEY_A) == GLFW_PRESS)
	{
		m_PlayerCon.MovePosition(glm::vec2(-1, 0), delta);
	}

	if (glfwGetKey(m_Window, GLFW_KEY_D) == GLFW_PRESS)
	{
		m_PlayerCon.MovePosition(glm::vec2(1, 0), delta);
	}
}

void Scene2::LateUpdate(float delta)
{
}

void Scene2::FixedUpdate(float fixedDelta)
{
}

