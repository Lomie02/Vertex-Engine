#include "MyScene.h"
#include <iostream>
#include "ResourceManager.h"
#include "SceneManager.h"
#include "gtx/compatibility.hpp"

#define GRID_SIZE 800

MyScene::MyScene() : VertexScene("DEFAULT_SCENE_VERTEX")
{
	Awake();
}

MyScene::MyScene(const char _name[]) : VertexScene(_name)
{	
	Awake();
}

MyScene::~MyScene()
{
	delete m_Object;
	delete m_Object2;

	delete m_TextRenderer;
	m_TextRenderer = nullptr;
}

void MyScene::Awake()
{
	m_TextRenderer = new VertexText2D(1920, 1080);
	m_TextRenderer->Load("Builds/fonts/arial.ttf", 24);
	m_Button = new Button("Play");
	m_Button->text = "Play";

	ResourceManager::LoadTexture("Builds/Textures/VertexLogo.png", "Vertex");
	ResourceManager::LoadTexture("Builds/Textures/Huggy.png", "boy1");

	m_Object = new GameObject("Huggy", true);
	m_Object2 = new GameObject("Home", true);

	m_MainCamera = new Camera();

	m_Button->SetActive(true);

	m_Object->texture = ResourceManager::GetTexture("Vertex");
	m_Object2->texture = ResourceManager::GetTexture("boy1");
	
	m_Manager.Register(m_Object);
	m_Manager.Register(m_Object2);
	
	m_Manager.Register(m_MainCamera);
	m_MainCamera->transform.rotation = 0;

	m_Object->SetCollider(AABB);
	m_Object2->SetCollider(AABB);

	m_Manager.RegisterUi(m_Button);
}

void MyScene::Start()
{

	m_Manager.GiveWindow(m_Window);
	m_Object->transform.size = glm::vec2(1,-1);

	m_Object->transform.position.x = 0;
	m_Object->transform.position.y = 0;

	m_Object2->transform.position.y = 20;
	m_Object2->transform.position.x = 20;

	m_Object2->transform.size = glm::vec2(2, -1);

	m_Button->transform.size.x = 2;
	m_Button->transform.size.y = -1;

	m_Button->text = "Ya boy";

	m_Button->SetParent(m_MainCamera);
	m_MainCamera->transform.position.x = 0;
	m_MainCamera->transform.position.y = 0;

	glClearColor(1,0,0,0);
}

void MyScene::Update(float delta)
{
	m_Manager.LogEvents(); // Dont Remove

	if (glfwGetKey(m_Window, GLFW_KEY_W) == GLFW_PRESS)
	{
		m_MainCamera->transform.position.y += 5 * delta;
	}

	if (glfwGetKey(m_Window, GLFW_KEY_S) == GLFW_PRESS)
	{
		m_MainCamera->transform.position.y -= 5 * delta;
	}

	if (glfwGetKey(m_Window, GLFW_KEY_A) == GLFW_PRESS)
	{
		m_MainCamera->transform.position.x -= 5 * delta;
	}

	if (glfwGetKey(m_Window, GLFW_KEY_D) == GLFW_PRESS)
	{
		m_MainCamera->transform.position.x += 5 * delta;
	}

	if (glfwGetKey(m_Window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		m_MainCamera->zoom += 1 * delta;
	}


	if (glfwGetKey(m_Window, GLFW_KEY_E) == GLFW_PRESS)
	{
		m_MainCamera->zoom -= 1 * delta;
	}
}

void MyScene::LateUpdate(float delta)
{
	m_Manager.ConfigureSystems();
	m_Manager.CollisionCheck();
}

void MyScene::FixedUpdate(float delta)
{
}

void MyScene::Rendering(Vertex2D* render)
{
	m_Manager.ConfigureRenderSystems(render);
	m_TextRenderer->Text2D("Vertex Engine", 200,200, 1,glm::vec3(0,0,1));
}