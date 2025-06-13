#include "Application.h"
#include <iostream>
#include <math.h>
#include "glm.hpp"
#include "Graphics.h"

#include "ResourceManager.h"
#include "ext.hpp"
#include "GameSettings.h"
#include "Shader.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui.h"
#include <direct.h>
#include "stb_image.h"

#include "VertexPrefs.h"
#include "VertexAccessExplorer.h"
#include "Time.h"
#include "Input.h"

#define GL_SILENCE_DEPRECATION
#pragma warning(disable : 4996);

Application::Application() // Creates the sound manager.
{
	m_SoundManager = createIrrKlangDevice();
}

Application::~Application()
{
	delete m_SceneManager;
	m_SceneManager = nullptr;

	delete m_Renderer;

	delete m_Settings;
	m_Settings = nullptr;

	if (m_VertexEditor != nullptr) {
		delete m_VertexEditor;
		m_VertexEditor = nullptr;
	}

	if (m_Mode) {

		delete m_Mode;
		m_Mode = nullptr;
	}

}

void Application::StartUp()
{
	const char* description = new char[512];

	if (!glfwInit()) {
		glfwGetError(&description);
		std::cout << "Window Failed ERROR: " << description << std::endl;
		ShutDown();
	}
	char other[] = PROJECT_NAME;

	char name[150] = "Vertex Engine 2 | Editor | Project: ";

	strcat(name, other);

	// Create the engines runtime mode. This allows systems to know if the engine is in editor or play
	m_Mode = new RunTimeMode();

	if (FINAL_BUILD)
	{
		m_Mode->EditorMode = EditorMode::PLAY;
	}

	if (m_Mode->EditorMode == EditorMode::PLAY)
	{
		if (FULLSCREEN) {
			GLFWmonitor* monitor;
			monitor = glfwGetPrimaryMonitor();

			m_GameWindow = glfwCreateWindow(PROJECT_RESOLUTION, PROJECT_NAME, monitor, nullptr);
		}
		else {
			m_GameWindow = glfwCreateWindow(PROJECT_RESOLUTION, PROJECT_NAME, nullptr, nullptr);
		}
	}
	else {
		m_GameWindow = glfwCreateWindow(1920, 1080, name, nullptr, nullptr);
	}

	if (!m_GameWindow) {
		glfwGetError(&description);
		std::cout << "Window Failed ERROR: " << description << std::endl;
		ShutDown();
	}

	srand((unsigned)time(NULL));
	glfwMakeContextCurrent(m_GameWindow);

	if (!gladLoadGL()) {
		ShutDown();
	}
	m_IsRunning = true;

	ResourceManager::LoadShader("Builds/Shaders/sprite.vs", "Builds/Shaders/sprite.frag", nullptr, "sprite");

	ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);

	Shader Shader = ResourceManager::GetShader("sprite");
	m_Renderer = new Vertex2D(Shader);

	m_SceneManager = new SceneManager();

	char other1[20] = PROJECT_ICON;
	char name1[100] = "Builds/Textures/Icons/";

	strcat(name1, other1);
	Texture2D data;

	int width;
	int height;

	int channels;
	unsigned char* picture;


	if (m_Mode)
	{
		picture = stbi_load(name1, &width, &height, &channels, 4);
	}
	else {

		picture = stbi_load("Engine/Icons/vertex_logo.png", &width, &height, &channels, 4);
	}

	// Load Gizmo Textures
	ResourceManager::LoadTexture("Engine/Gizmos/Unnamed.png", "Gizmo_Camera");
	ResourceManager::LoadTexture("Engine/Gizmos/gizmo_centre.png", "Gizmo_Center");
	ResourceManager::LoadTexture("Engine/Gizmos/gizmo_cords.png", "Gizmo_Cords");

	GLFWimage Icons[1];
	Icons[0].height = height;
	Icons[0].width = width;
	Icons[0].pixels = picture;

	glfwSetWindowIcon(m_GameWindow, 1, Icons);

	if (VSYNC)
	{
		glfwSwapInterval(true ? 1 : 0);
	}
	else {
		glfwSwapInterval(false ? 1 : 0);
	}

	m_Settings = new BootUpContainer();

	/*m_ApplicationFramesPerSecond = 0;
	m_frames = 0;
	m_UnScaledDelta = 0;
	m_fpsInterval = 0;
	m_prevTime = (float)glfwGetTime();*/

	Time::PrepareDeltaCalculations();

	FolderCreation();
	ExternalResources(); //Load External files.

	m_TransitionScene = new VertexTransitions("Transitions");
	m_SceneManager->AddScene(m_TransitionScene);

	m_TransitionScene->GiveWindow(m_SceneManager);


	if (m_UsingRenderer == Vertex_2D)
	{
		m_Settings->m_UseDefaultRenderer = true;
	}
	else if (m_UsingRenderer == Tension_2D)
	{
		m_Settings->m_UseDefaultRenderer = false;
	}
	m_Settings->m_AutoDeletePointers = AUTO_DELETE_ASSET_POINTERS;
	m_Settings->m_TransparentSortingAlgo = Insertion_Sort;

	/*
		CREATE THE VERTEX EDITOR
	*/

	if (m_Mode->EditorMode == EditorMode::EDITOR && !FINAL_BUILD)
	{
		m_EditorFullScreen = false;
		m_VertexEditor = new VertexEngineEditor();
		m_VertexEditor->CreateEditorLayout(m_GameWindow, m_SceneManager);
		m_VertexEditor->GiveEditorMode(m_Mode);

		std::cout << "Vertex Message: Editor Succeded." << std::endl;
	}

	for (int i = 0; i < m_SceneManager->m_SceneList.size(); i++)
	{
		m_SceneManager->m_SceneList.at(i)->GetAssets()->BootUpAll(m_Settings);
	}

	std::cout << "Vertex Message: Start Up Succeded." << std::endl;



	m_SceneManager->EngineState(m_Mode);

	glClearColor(BACKGROUND_COLOUR);
}

void Application::Start()
{
	Input::Init(m_GameWindow);

	ExternalResources();

	if (USE_VERTEX_WORK_SPACE) {
		m_WorkSpaceEditor.SceneCreation();
		SetUpWorkSpaceScenes(); // Set Up scenes from the workspace if being used.
	}
	else {
		SceneSetUp(); //Start setting up all scenes in engine.
	}

	BeginSceneSystemAssigning();


	for (int i = 0; i < m_SceneManager->m_SceneList.size(); i++) {
		m_SceneManager->m_SceneList.at(i)->GetAssets()->AssignSoundSystem(m_SoundManager);
	}

	switch (m_Mode->EditorMode) {
	case EditorMode::PLAY:
		m_SceneManager->SetActiveScene(0);
		break;

	case EditorMode::EDITOR:
		m_SceneManager->SetActiveScene(1);
		break;
	}

	m_SceneManager->StartUpScenes();

	if (m_Mode->EditorMode == EditorMode::EDITOR) {
		AssetPipelineManager::ScanFolderForTextures();
		m_VertexEditor->PopulateDesk();
	}

	std::cout << "Vertex Message: Start Succeded." << std::endl;
}

void Application::Update()
{
	if (m_Mode->EditorMode == EditorMode::PLAY || m_Mode->EditorMode == EditorMode::EDITOR_PLAY) {

		// Calculate delta time.
		Time::ConfigureDeltaTime();

		// Update the input system before any other update occurs.
		Input::UpdateInput();

		if (m_FinishedSceneSetUpStage)
			m_SceneManager->UpdateScenes(Time::GetDeltaTime()); // Update regular loop

		while (Time::GetFixedDeltaTime() >= Time::GetTimeStep())
		{
			m_SceneManager->UpdateFixedScenes(Time::GetTimeStep()); // Add timestep to Fixed & Late Update Loop.
			m_SceneManager->GetCurrentScene()->GetAssets()->ConfigurePhysics(Time::GetTimeStep());
			Time::ResetFixedDelta();
		}

		m_SceneManager->GetCurrentScene()->GetAssets()->ConfigureSystems(); // Update asset managers systems
	}
}

void Application::SetUpWorkSpaceScenes()
{
	// Adds scenes from the workspace to the scene manager.
	for (int i = 0; i < m_WorkSpaceEditor.GrabWorkSpaceScenes().size(); i++) {

		m_SceneManager->AddScene(m_WorkSpaceEditor.GrabWorkSpaceScenes().at(i));
	}
}

void Application::EditorSpacer(int _spaces)
{
	for (int i = 0; i < _spaces; i++) {
		ImGui::Spacing();
	}
}

void Application::BeginSceneSystemAssigning()
{
	if (m_SceneManager->m_SceneList.size() == 0) {
		std::cout << "VERTEX ERROR: No Scenes have been created! Auto shutdown begining." << std::endl;
		ShutDown();
	}

	// Give alls scenes the current Runtime mode, glfw window & the scene manager itself.
	for (int i = 0; i < m_SceneManager->m_SceneList.size(); i++)
	{
		m_SceneManager->m_SceneList.at(i)->GetAssets()->AssignMode(m_Mode);
		m_SceneManager->m_SceneList.at(i)->GetAssets()->GiveWindow(m_GameWindow);
		m_SceneManager->m_SceneList.at(i)->GetAssets()->GiveSceneManager(m_SceneManager);
	}

	m_FinishedSceneSetUpStage = true;
}


void Application::RenderAll()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_SceneManager->GetCurrentScene()->GetAssets()->ConfigureRenderSystems(m_Renderer);

	if (m_Mode->EditorMode != EditorMode::PLAY)
	{
		m_SceneManager->GetCurrentScene()->GetAssets()->Gizmos(m_Renderer);
	}

	if (m_Mode->EditorMode == EditorMode::EDITOR && !m_EditorFullScreen || m_Mode->EditorMode == EditorMode::EDITOR_PLAY && !m_EditorFullScreen || m_Mode->EditorMode == EditorMode::EDITOR_PAUSED && !m_EditorFullScreen)
	{
		m_VertexEditor->RenderEditorDisplays();
	}

	glfwSwapBuffers(m_GameWindow);
	glfwPollEvents();
}

/// <summary>
/// Quits the application.
/// </summary>
void Application::Quit()
{
	m_IsRunning = false;
}

//=============================================== Add your scenes Here
void Application::SceneSetUp()
{
	m_Scene = new Scene2("My Other Scene");

	m_Scene->GiveWindow(m_GameWindow);
	m_SceneManager->AddScene(m_Scene);
}

void Application::ShutDown()
{
	if (m_Mode->EditorMode == EditorMode::EDITOR || m_Mode->EditorMode == EditorMode::EDITOR_PLAY)
	{
		m_VertexEditor->CleanUpGui();
	}

	Cursor::Show(m_GameWindow);

	m_SceneManager->CleanUpSceneManagerAssets();
	glfwSetInputMode(m_GameWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void Application::FolderCreation()
{
	int success;
	success = mkdir("Builds");
	success = mkdir("Builds/Textures");
	success = mkdir("Builds/Textures/Icons");

	success = mkdir("Builds/Scene_Assets");
	success = mkdir("Builds/Shaders");
	success = mkdir("Builds/Audio");
	success = mkdir("Builds/Data");
	success = mkdir("Builds/Data/0x021_0KAW");

	std::cout << "Vertex Message: Completed file creation." << std::endl;
}

void Application::ExternalResources() // Load all your external files such as textures, fonts & audio files.
{
	AssetPipelineManager::Init();
}
