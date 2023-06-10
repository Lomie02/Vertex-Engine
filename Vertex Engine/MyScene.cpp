#include "MyScene.h"
#include <iostream>
#include "ResourceManager.h"
#include "SceneManager.h"
#include "VertexPrefs.h"
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
	m_Object = nullptr;

	delete m_Object2;
	m_Object2 = nullptr;

	delete m_MyText;
	m_MyText = nullptr;

	delete m_DummyCamera;
	m_DummyCamera = nullptr;

	delete m_Body;
	m_Body = nullptr;

	delete m_Button;
	m_Button = nullptr;

	delete m_MainCamera;
	m_MainCamera = nullptr;
}

void MyScene::Awake()
{
	m_Button = new Button("Play");
	m_Button->text = "Play";

	m_MyText = new Text();
	m_Manager.Register(m_MyText);

	m_MyText->text = "Vertex Engine";

	ResourceManager::LoadTexture("Builds/Textures/PowerIcon.png", "Vertex");
	ResourceManager::LoadTexture("Builds/Textures/VertexLogo.png", "boy1");
	ResourceManager::LoadTexture("Builds/Textures/UI_Canvas.png", "Canvas");

	m_Object = new GameObject("Huggy", true);
	m_Object2 = new GameObject("Animation", true);
	//m_Canvas = new GameObject("Canvas", true);

	m_MainCamera = new Camera();
	m_DummyCamera = new Camera("Boy Camera");

	m_Button->SetActive(true);
	m_Button->transform.position.x = 10;

	m_Button->transform.size = glm::vec2(100, -100);

	m_Object->material.baseTexture = ResourceManager::GetTexture("Vertex");
	m_Object2->material.baseTexture = ResourceManager::GetTexture("boy1");
	//m_Canvas->material.baseTexture = ResourceManager::GetTexture("Canvas");

	m_Object->material.surface = Transparent;

	m_Button->material.baseTexture = ResourceManager::GetTexture("boy1");

	m_Object2->material.colour = glm::vec4(1, 0, 0, 0.5f);
	m_Object->material.surface = Transparent;

	m_Manager.Register(m_Object2);
	m_Manager.Register(m_Object);

	m_Object2->layer = layer_07;

	m_Manager.Register(m_MainCamera);
	m_Manager.Register(m_DummyCamera);

	m_MainCamera->transform.rotation = 0;

	m_Manager.Register(m_Button);
	//m_Body = new RigidBody("Yep");

	//m_Body->material.baseTexture = ResourceManager::GetTexture("boy1");
	//m_Body->material.colour.r = 1;
	//m_Manager.Register(m_Body);

	//m_Canvas->transform.size = glm::vec2(1280, 720);
	//m_Anim->SetMaster(m_Object2);
	//m_Manager.Register(m_Canvas);
}

void MyScene::Start()
{
	m_Button->material.colour = glm::vec4(1,0,0,1);
	m_Object->transform.size = glm::vec2(1, 1);

	m_Object->transform.position.x = 0;
	m_Object->transform.position.y = 0;

	//m_Object2->transform.position.y = 20;
	//m_Object2->transform.position.x = 20;

	//m_Body->transform.position.x = 0;
	//m_Body->transform.position.y = 0;

	//m_Body->transform.size = glm::vec2(2, 2);

	m_Object2->transform.size = glm::vec2(2, 1);

	m_Button->transform.size.x = 5;
	m_Button->transform.size.y = 3;

	m_Button->text = "Play";

	m_Button->transform.position.x = 5.0f;
	m_Button->transform.position.y = 5.0f;

	m_MainCamera->transform.position.x = 10;
	m_MainCamera->transform.position.y = 10;

	m_MyText->transform.position.x = 10;
	m_MyText->transform.position.y = 10;

	glClearColor(0.2f, 0.2f, 0.2f, 0);

}

void MyScene::Update(float delta)
{
	//m_Object2->transform.position.x = m_Manager.GetMousePosition().x;
	//m_Object2->transform.position.y = -m_Manager.GetMousePosition().y * delta;
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

	if (glfwGetKey(m_Window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		GameObject cast;

		if (m_Manager.Raycast2D(m_Manager.GetMousePosition(), glm::vec2(0, 1), cast, 5.0f))
		{
			std::cout << cast.name << std::endl;
		}
	}

	if (m_Manager.GetMousePosition().x < m_Object2->transform.GetCentre().x + 5 * m_Object2->transform.scale && m_Manager.GetMousePosition().x > m_Object2->transform.GetCentre().x - 5 * m_Object2->transform.scale) {
		std::cout << "X Collision" << std::endl;
	}

	if (m_Button->Pressed())
	{
		std::cout << "PRESSED BUTTON" << std::endl;
		m_Button->CloseEvent();
	}
}

void MyScene::LateUpdate(float delta) //TODO: Automate all below
{
}

void MyScene::FixedUpdate(float fixedDelta)
{
}
