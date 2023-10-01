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

	delete m_Object0;
	m_Object0 = nullptr;

	delete m_MainCamera;
	m_MainCamera = nullptr;

	delete m_ButtonTest;
	m_ButtonTest = nullptr;

	delete m_Title;
	m_Title = nullptr;

	delete m_FlipbookAnimation;
	m_FlipbookAnimation = nullptr;

	delete m_Egg;
	m_Egg = nullptr;

	delete m_EggFlipBook;
	m_EggFlipBook = nullptr;

	delete m_Controller;
	m_Controller = nullptr;
}

void Scene2::Awake()
{
	m_Object2 = new GameObject("Chimken");
	m_Object0 = new GameObject("Myring");
	m_Block = new GameObject("Block");

	m_Block->material.baseTexture = ResourceManager::LoadTexture("Builds/Textures/block.png", "block");

	m_Egg = new GameObject("Egg");
	m_Controller = new PlayerController();

	m_Object0->material.m_KeepAspect = true;

	m_Object2->material.baseTexture = ResourceManager::LoadTexture("Builds/Textures/Chimken.png", "Chimken");
	ResourceManager::LoadTexture("Builds/Textures/Chimken_01.png", "Chimken1");

	m_Object2->material.baseTexture = ResourceManager::GetTexture("Chimken1");

	m_Object2->material.surface = Transparent;
	m_Object0->material.surface = Transparent;


	// Egg Texture
	ResourceManager::LoadTexture("Builds/Textures/Egg.png", "Egg");
	ResourceManager::LoadTexture("Builds/Textures/EggOpen.png", "EggOpen");
	m_Egg->material.baseTexture = ResourceManager::GetTexture("Egg");
	m_Egg->material.surface = Transparent;

	m_Egg->transform.position.x = -4;
	m_Egg->transform.position.y = -1;

	//====================

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

	m_Title = new Text();
	m_Canvas = new Canvas();

	m_Canvas->Add(m_Title);
	m_Sprite = new Sprite("ye");
	m_Sprite->material.baseTexture = ResourceManager::GetTexture("Chimken");
	m_Sprite->material.m_KeepAspect = true;

	m_Canvas->Add(m_Sprite);
	m_Manager.Register(m_Canvas);

	m_Title->m_FontSize = 90;

	m_Title->ChangeFont("Open 24 Display St");

	m_Title->transform.position.x = -7;
	m_Title->transform.position.y = 5;
	m_Title->text = "";

	m_Title->material.colour.r = 1;
	m_Title->material.colour.g = 1;
	m_Title->material.colour.b = 1;

	// Setup animation test

	m_FlipbookAnimation = new Flipbook();
	m_EggFlipBook = new Flipbook();

	m_Manager.Register(m_FlipbookAnimation);
	m_Manager.Register(m_EggFlipBook);

	m_EggFlipBook->SetMaster(m_Egg);

	m_FlipbookAnimation->SetMasterMode(Multiple);

	m_FlipbookAnimation->SetMaster(m_Object0);
	m_FlipbookAnimation->SetMaster(m_Object2);

	m_IdleClip.m_Name = "Idle";

	StartFlipbookSetUp();
	m_FlipbookAnimation->AddFrame(m_IdleClip);

	m_EggClip.m_PlaySpeed = 2.0f;
	m_EggFlipBook->AddFrame(m_EggClip);

	m_Animation = new Animator();

	m_Animation->SetMaster(m_Object2);
	m_Object2->transform.position = glm::vec2(6, 0);
	m_Animation->AddKeyFrame();
	m_Object2->transform.position = glm::vec2(1, 0);
	m_Object2->material.colour.a = 0.1f;
	m_Animation->AddKeyFrame();
	m_Object2->transform.position = glm::vec2(6, 0);
	m_Object2->material.colour.a = 1.0f;
	m_Animation->AddKeyFrame();

	//====================
	SetupButton();

	m_Manager.Register(m_Egg);
	m_ButtonTest->SetActive(false);

	m_Controller->AssignPlayer(m_Object0);
	m_Manager.Register(m_Controller);

	m_Block->InstanceMime("Yeah", glm::vec2(3,3));

	m_Manager.Register(m_Block);
	// ANimator Setup
	m_Egg->material.m_KeepAspect = true;
	m_Egg->material.TransparencyBlend = Alpha;

	m_Animation->WrapMode(Loop);
	m_Manager.Register(m_Animation);

	m_UserCamera = new Camera("UserCamera");
	m_Manager.RegisterUserInterfaceCamera(m_UserCamera);
}

void Scene2::Start()
{
	m_Manager.GiveWindow(m_Window);
	glClearColor(0.3, 0.3, 0.3, 1.0);

	m_FlipbookAnimation->AdjustClipPlaySpeed("Idle", 20);

	m_FlipbookAnimation->Play();
	m_EggFlipBook->Play();

	m_Title->text = "Chimken Party";
	m_Title->SetActive(true);

	m_Animation->Play();
	m_Object0->transform.position.x = -7;
	m_Controller->SetWeight(0);
	m_Controller->SetSpeed(20);
}

void Scene2::Update(float delta)
{
	if (glfwGetKey(m_Window,GLFW_KEY_W) == GLFW_PRESS) {
		m_Controller->MovePosition(glm::vec2(0,1), delta);
	}
	if (glfwGetKey(m_Window, GLFW_KEY_S) == GLFW_PRESS) {
		m_Controller->MovePosition(glm::vec2(0, -1), delta);
	}
	if (glfwGetKey(m_Window, GLFW_KEY_D) == GLFW_PRESS) {
		m_Controller->MovePosition(glm::vec2(1, 0), delta);
	}
	if (glfwGetKey(m_Window, GLFW_KEY_A) == GLFW_PRESS) {
		m_Controller->MovePosition(glm::vec2(-1, 0), delta);
	}

	// Timer for text
	//m_TitleTimer += 1 * delta;

	//if (m_TitleTimer > m_Duration) {

	//	m_TitleTimer = 0;

	//	if (m_Title->GetActive())
	//		m_Title->SetActive(false);
	//	else
	//		m_Title->SetActive(true);
	//}
}

void Scene2::LateUpdate(float delta)
{
}

void Scene2::FixedUpdate(float fixedDelta)
{
}

void Scene2::StartFlipbookSetUp()
{
	m_IdleClip.m_Loop = true;
	m_EggClip.m_Loop = true;

	m_EggClip.m_Frames.push_back(ResourceManager::GetTexture("Egg"));
	m_EggClip.m_Frames.push_back(ResourceManager::GetTexture("EggOpen"));

	m_IdleClip.m_Frames.push_back(ResourceManager::GetTexture("Chimken"));
	m_IdleClip.m_Frames.push_back(ResourceManager::GetTexture("Chimken1"));
}

void Scene2::SetupButton()
{

	m_ButtonTest->transform.size.x = 10;
	m_ButtonTest->transform.size.y = 3;

	m_ButtonTest->transform.position.x = -5;
	m_ButtonTest->SetOffset(glm::vec2(-1, 0.3f));

	m_ButtonTest->text = "Vertex Ahh Button";
}

