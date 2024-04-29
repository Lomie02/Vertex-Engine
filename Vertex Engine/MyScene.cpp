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

	m_SceneManager = GetAssets().GetSceneManager();
	m_NavGrid = new NavigationGrid();
	NavigationNode* nodes[11 * 11];

	for (int y = 0; y < 11; y++) {
		for (int x = 0; x < 11; x++) {
			nodes[y * 11 + x] = m_NavGrid->AddNode(glm::vec2(100 + x * GRAPH_X, 100 + y * GRAPH_Y));
			nodes[y * 11 + x]->m_IsEnabled = true;
		}
	}

	nodes[17]->m_IsEnabled = false;
	nodes[12]->m_IsEnabled = false;
	nodes[13]->m_IsEnabled = false;
	nodes[14]->m_IsEnabled = false;
	nodes[23]->m_IsEnabled = false;
	nodes[25]->m_IsEnabled = false;
	nodes[20]->m_IsEnabled = false;
	nodes[34]->m_IsEnabled = false;
	nodes[33]->m_IsEnabled = false;
	nodes[60]->m_IsEnabled = false;
	nodes[63]->m_IsEnabled = false;
	nodes[62]->m_IsEnabled = false;
	nodes[51]->m_IsEnabled = false;
	nodes[54]->m_IsEnabled = false;
	nodes[40]->m_IsEnabled = false;
	nodes[52]->m_IsEnabled = false;
	nodes[80]->m_IsEnabled = false;
	nodes[59]->m_IsEnabled = false;
	nodes[79]->m_IsEnabled = false;
	nodes[76]->m_IsEnabled = false;

	for (int y = 0; y < 11; y++)
	{
		for (int x = 0; x < 11; x++)
		{
			NavigationNode** n = &nodes[y * 11 + x];


			if (x < 10) {

				m_NavGrid->AddConnection(*n, *(n + 1));
			}

			if (x > 0) {

				m_NavGrid->AddConnection(*n, *(n - 1));
			}

			if (y < 10) {

				m_NavGrid->AddConnection(*n, *(n + 11));
			}

			if (y > 0) {

				m_NavGrid->AddConnection(*n, *(n - 11));
			}
		}
	}

	m_StaticAnimation = new Flipbook();
	AssignTextures();
	m_Manager.Register(m_StaticAnimation);

	m_MyText = new Text();
	m_Agent = new NavAgent(m_NavGrid);

	m_Manager.Register(m_Agent);
	m_Agent->transform.position.x = 1;
	m_Agent->transform.position.y = 1;

	m_Manager.Register(m_Static);
	m_Manager.Register(m_MansFace);
	m_Manager.Register(m_LadysFace);

	m_Manager.Register(m_MainCamera);
	m_Manager.Register(m_MyText);

	m_Manager.Register(m_MansFace);

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

	m_Agent->material.baseTexture = ResourceManager::GetTexture("Face");
	m_Static->material.surface = Transparent;
	glClearColor(0.0f, 0.0f, 0.0f, 0);
}

void MyScene::Update(float delta)
{

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
