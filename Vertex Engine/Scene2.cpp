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

	delete m_Camera01;
	m_Camera01 = nullptr;

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

	delete m_Camera02;
	m_Camera02 = nullptr;
}

void Scene2::Awake()
{
	m_Object2 = new GameObject("Chimken");
	m_Object0 = new GameObject("Mr Chicken");

	m_Camera01 = new GameObject("Camera 1");
	m_Camera02 = new GameObject("Camera 2");

	m_Camera01->AddComponent<Camera>();
	m_Camera02->AddComponent<Camera>();

	m_SceneManager = GetAssets()->GetSceneManager();

	m_Egg = new GameObject("Egg");
	m_Controller = new PlayerController();

	m_Object0->material.m_KeepAspect = true;

	m_Object2->material.AlbedoMap = ResourceManager::LoadTexture("Builds/Textures/Girl_01.png", "Chimken");
	ResourceManager::LoadTexture("Builds/Textures/Girl_02.png", "Chimken1");

	m_Object2->material.AlbedoMap = ResourceManager::GetTexture("Chimken1");

	m_Object2->material.surface = Transparent;
	m_Object0->material.surface = Transparent;

	// Egg Texture
	ResourceManager::LoadTexture("Builds/Textures/Block.png", "Egg");
	ResourceManager::LoadTexture("Builds/Textures/EggOpen.png", "EggOpen");
	m_Egg->material.AlbedoMap = ResourceManager::GetTexture("Egg");
	m_Egg->material.surface = Transparent;

	m_Egg->transform.position.x = -4;
	m_Egg->transform.position.y = -1;

	//====================

	m_Object0->transform.position.x = 3;
	m_Object0->layer = 1;
	m_Object0->transform.SetSize(5, -5);
	m_Object2->layer = 3;

	m_Object0->material.AlbedoMap = ResourceManager::GetTexture("Chimken");
	m_Manager->Register(m_Object2);
	m_Manager->Register(m_Object0);


	m_Manager->Register(m_Camera01);

	m_ButtonTest = new Button("Goofy Button");

	m_Manager->Register(m_ButtonTest);

	m_Title = new Text();
	m_CounterText = new Text();

	//========================================
	m_Sprite = new Sprite("EggIcon");
	m_Sprite->material.AlbedoMap = ResourceManager::GetTexture("Egg");
	m_Sprite->material.m_KeepAspect = true;
	m_Sprite->material.surface = Transparent;

	m_Sprite->transform.position = glm::vec2(-18, -4);

	m_Sprite->transform.size.x = 5;
	m_Sprite->transform.size.y = 5;
	//======================================== Counter Text

	m_CounterText->m_FontSize = 17;
	m_CounterText->material.colour = glm::vec4(1, 1, 1, 1);
	m_CounterText->text = "0";
	m_CounterText->transform.position = glm::vec2(-15, -5);
	//========================================

	m_Title->m_FontSize = 24;

	m_Title->ChangeFont("Arial");

	m_Title->transform.position.x = -7;
	m_Title->transform.position.y = 5;
	m_Title->text = "";

	m_Title->material.colour.r = 1;
	m_Title->material.colour.g = 1;
	m_Title->material.colour.b = 1;


	// Setup animation test

	m_FlipbookAnimation = new Flipbook();
	m_EggFlipBook = new Flipbook();

	m_Manager->Register(m_FlipbookAnimation);
	m_Manager->Register(m_EggFlipBook);

	m_EggFlipBook->SetMaster(m_Egg);

	m_FlipbookAnimation->SetMasterMode(Multiple);

	m_FlipbookAnimation->SetMaster(m_Object0);
	m_FlipbookAnimation->SetMaster(m_Object2);
	m_IdleClip.m_Name = "Idle";
	m_IdleClip.m_PlaySpeed = 0.05f;

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

	// ====================================== Bodies

	m_Body = new RigidBody("Blud");
	m_Decoy = new RigidBody("Blud");
	m_BlockBody = new RigidBody("Block");

	m_Body->material.AlbedoMap = ResourceManager::GetTexture("Egg");
	m_BlockBody->material.AlbedoMap = ResourceManager::GetTexture("Egg");
	m_BlockBody->material.AlbedoMap = ResourceManager::GetTexture("Egg");

	m_Body->transform.position = glm::vec2(9, 0);
	m_Decoy->transform.position = glm::vec2(2, 0);

	m_BlockBody->transform.position = glm::vec2(6, -5);

	m_BlockBody->SetMass(0);
	m_Body->SetMass(1);
	 
	m_BlockBody->transform.SetSize(20.0f, 2.0f);
	m_Decoy->transform.SetSize(2.0f, 2.0f);

	m_Body->Init(new btBoxShape(btVector3(m_Body->transform.GetSize().x/ 2, m_Body->transform.GetSize().y / 2, 0.1f)), 1);
	m_Decoy->Init(new btBoxShape(btVector3(m_Decoy->transform.GetSize().x * m_Decoy->transform.scale / 2, m_Decoy->transform.GetSize().y * m_Decoy->transform.scale / 2, 0.1f)), 1);
	m_BlockBody->Init(new btBoxShape(btVector3(m_BlockBody->transform.GetSize().x / 2, m_BlockBody->transform.GetSize().y / 2, 0.1f)), 0);

	m_Manager->Register(m_Body);
	m_Manager->Register(m_Decoy);
	m_Manager->Register(m_BlockBody);

	m_Manager->SetWorldGravity(glm::vec3(0.f, -9.8f, 0.0f));

	// ====================================== Bodies
	//====================
	SetupButton();

	m_TexureRender = new RenderTexture(1920, 1080);


	m_CamMain = m_Camera01->GetComponenet<Camera>();
	m_CamRender = m_Camera02->GetComponenet<Camera>();

	m_CamRender->SetDisplay(1);
	m_CamMain->zoom = 20;
	m_CamRender->zoom = 20;
	m_CamRender->renderTexture = m_TexureRender;

	m_Manager->Register(m_Camera01);
	m_Manager->Register(m_Camera02);

	m_Object2->AddComponent<DebugComp>();
	m_Object2->AddComponent<DebugComp>();
	m_Object2->AddComponent<DebugComp>();

	m_Object0->material.AlbedoMap.ID = m_TexureRender->GetTexture();

	std::cout << "Comp List: " << m_Object2->FindComponentsOfType<DebugComp>().size() << std::endl;

	m_Manager->Register(m_Egg);
	m_ButtonTest->SetActive(false);

	m_Controller->AssignPlayer(m_Object0);
	m_Manager->Register(m_Controller);

	// ANimator Setup
	m_Egg->material.m_KeepAspect = true;
	m_Egg->material.TransparencyBlend = Alpha;

	m_Animation->WrapMode(Loop);
	m_Manager->Register(m_Animation);

}

void Scene2::Start()
{
	m_Manager->GiveWindow(m_Window);
	glClearColor(0.3, 0.3, 0.3, 1.0);

	m_FlipbookAnimation->AdjustClipPlaySpeed("Idle", 20);

	m_FlipbookAnimation->Play();
	m_EggFlipBook->Play();

	m_Title->text = "Vertex Engine 2";
	m_Title->SetActive(true);

	m_Animation->Play();
	m_Object0->transform.position.x = -7;
	m_Controller->SetWeight(0);
	m_Controller->SetSpeed(5);

}

void Scene2::Update(float delta)
{
	if (glfwGetKey(m_Window, GLFW_KEY_W) == GLFW_PRESS) {
		m_Body->ApplyForce(glm::vec3(0, 1, 0) * 10.0f);
	}
	if (glfwGetKey(m_Window, GLFW_KEY_S) == GLFW_PRESS) {
		m_Body->ApplyForce(glm::vec3(0, -1, 0) * 10.0f);
	}
	if (glfwGetKey(m_Window, GLFW_KEY_D) == GLFW_PRESS) {
		m_Body->ApplyForce(glm::vec3(1, 0, 0) * 10.0f);
	} 
	if (glfwGetKey(m_Window, GLFW_KEY_A) == GLFW_PRESS) {
		m_Body->ApplyForce(glm::vec3(-1, 0, 0) * 10.0f);
	}

	if (glfwGetKey(m_Window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		m_Body->ApplyForce(m_Body->GetForward() * 50.0f);
	}

	if (glfwGetKey(m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwTerminate();
	}
}

void Scene2::LateUpdate(float delta)
{
}

void Scene2::FixedUpdate(float fixedDelta)
{
	//std::cout << "Pos: " << m_Body->transform.position.x << " | " << m_Body->transform.position.y << std::endl;
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

