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
	delete m_LadysFace;
	m_LadysFace = nullptr;

	delete m_MainCamera;
	m_MainCamera = nullptr;

	delete m_MyText;
	m_MyText = nullptr;
}

void MyScene::Awake()
{
	m_LadysFace = new GameObject("Picture");
	m_MainCamera = new Camera("Camera");
	m_Static = new GameObject("Static");
	m_MansFace = new GameObject("man");

	m_InterfaceCamera = new Camera("Interface Camera");

	m_SceneManager = GetAssets().GetSceneManager();

	m_StaticAnimation = new Flipbook();
	AssignTextures();
	m_Manager.Register(m_StaticAnimation);

	m_MyText = new Text();

	m_Manager.Register(m_Static);
	m_Manager.Register(m_MansFace);
	m_Manager.Register(m_LadysFace);

	m_Manager.Register(m_MainCamera);
	m_Manager.Register(m_MyText);

	m_Manager.Register(m_MansFace);
	m_Manager.RegisterUserInterfaceCamera(m_InterfaceCamera);

	m_MyText->text = "ANOMOLY CONTAINMENT";
	m_MyText->ChangeFont("Open 24 Display St");
	m_MyText->material.colour.r = 1.0f;

	m_Static->transform.size.x = 40;
	m_Static->transform.size.y = 50;

	m_Static->material.colour.a = 0.1f;

	m_SoundEffect = new AudioSource();

	m_SoundEffect->AudioClip("Builds/Audio/Im_The_Homelander.wav");
	m_Manager.Register(m_SoundEffect);
}

void MyScene::Start()
{
	m_MyVolume.ChromaticAberation.ChromaticIntensity = 0.01f;
	m_MyVolume.ChromaticAberation.ChromaticEnabled = true;
	//m_Manager.Register(m_MyVolume);
	m_SoundEffect->Play();
	m_StaticAnimation->AdjustClipPlaySpeed("static", 50);
	m_StaticAnimation->Play();

	m_Static->material.surface = Transparent;
	glClearColor(0.0f, 0.0f, 0.0f, 0);
}

void MyScene::Update(float delta)
{
	if (glfwGetKey(m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwTerminate();
	}
}

void MyScene::LateUpdate(float delta) //TODO: Automate all below
{
}

void MyScene::FixedUpdate(float fixedDelta)
{
}

void MyScene::AssignTextures() // Assign all textures
{
	ResourceManager::LoadTexture("Builds/Textures/Face.jpg", "Face");
	ResourceManager::LoadTexture("Builds/Textures/Man.jpg", "ManFace");

	ResourceManager::LoadTexture("Builds/Textures/Animation/state01.png", "Static");
	ResourceManager::LoadTexture("Builds/Textures/Animation/state02.png", "Static1");
	ResourceManager::LoadTexture("Builds/Textures/Animation/state03.png", "Static3");

	m_LadysFace->material.baseTexture = ResourceManager::GetTexture("Face");
	m_MansFace->material.baseTexture = ResourceManager::GetTexture("ManFace");
	m_Static->material.baseTexture = ResourceManager::GetTexture("Static");

	m_Clip.m_Name = "static";

	m_Clip.m_Frames.push_back(ResourceManager::GetTexture("Static"));
	m_Clip.m_Frames.push_back(ResourceManager::GetTexture("Static1"));
	m_Clip.m_Frames.push_back(ResourceManager::GetTexture("Static3"));

	m_Clip.m_Loop = true;

	m_StaticAnimation->AddFrame(m_Clip);
	m_StaticAnimation->SetMaster(m_Static);
}
