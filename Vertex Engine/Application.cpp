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

#define GL_SILENCE_DEPRECATION
#pragma warning(disable : 4996);

//==========================================
Application::Application()
{
}

Application::~Application()
{
	delete m_SceneManager;
	m_SceneManager = nullptr;

	delete m_Renderer;

	delete m_Scene;
	m_Scene = nullptr;


	//========================== Delete your scenes here!
	/*
		HOW TO DELETE SCENES CORRECTLY:

		delete YourScene;
		YourScene = nullptr;
	*/

	delete m_SecondScene;
	m_SecondScene = nullptr;
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

	char name[150] = "Vertex Engine | Editor | Project: ";

	strcat(name, other);

	if (m_Mode == PLAY)
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

	if (VSYNC)
	{
		glfwSwapInterval(true ? 1 : 0);
	}
	else {
		glfwSwapInterval(false ? 1 : 0);
	}

	m_fps = 0;
	m_frames = 0;
	m_deltaTime = 0;
	m_fpsInterval = 0;
	m_prevTime = (float)glfwGetTime();

	FolderCreation();
	SceneSetUp();
	m_SceneManager->SetUpWindow(m_GameWindow);
	// Load UI Textures
	ResourceManager::LoadTexture("Builds/Textures/UI_Button.png", "UI_Button");

	if (FINAL_BUILD)
	{
		m_Mode = PLAY;
	}

	if (m_Mode == EDITOR || m_Mode == EDITOR_PLAY)
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		ImGui_ImplGlfw_InitForOpenGL(m_GameWindow, true);
		ImGui_ImplOpenGL3_Init("#version 330");

		ImGui::StyleColorsDark();

		glViewport(299.973f, 349.968f, 1280, 720);
		glScissor(299.973f, 349.968f, 1280, 720);
		glEnable(GL_SCISSOR_TEST);

		UpdateEditorMode();
		std::cout << "Vertex Message: Editor Succeded." << std::endl;
	}
	
	std::cout << "Vertex Message: Start Up Succeded." << std::endl;

	glClearColor(BACKGROUND_COLOUR);
}

void Application::Start()
{
	m_SceneManager->StartUpScenes();
	std::cout << "Vertex Message: Start Succeded." << std::endl;
}

void Application::Update()
{
	if (m_Mode == PLAY || m_Mode == EDITOR_PLAY) {

		double currTime = (float)glfwGetTime();
		m_deltaTime = currTime - m_prevTime;
		m_prevTime = currTime;

		m_frames++;
		m_fpsInterval += m_deltaTime;
		if (m_fpsInterval >= 1.0f)
		{
			m_fps = m_frames;
			m_frames = 0;
			m_fpsInterval -= 1.0f;
		}
		m_SceneManager->UpdateScenes(m_deltaTime);
	}
}

void Application::Editor()
{
	static bool ShowHelp;
	static const char* m_SceneList[MAX_SCENES];
	static const char* m_Assets[MAX_ASSETS];

	static const char* m_Cameras[MAX_CAMERAS];
	static const char* m_UI[80];

	static int Cameraselected = 0;
	static int CanvaSelected = 0;
	static int selected = 0;
	static int currentScene = 0;

	for (int i = 0; i < MAX_ASSETS; i++)
	{
		if (i < m_SceneManager->m_SceneList.at(m_SceneManager->GetActiveScene())->GetAssets().m_Objects.size())
		{
			m_Assets[i] = m_SceneManager->m_SceneList.at(m_SceneManager->GetActiveScene())->GetAssets().m_Objects.at(i)->name;
		}
		else
		{
			m_Assets[i] = " ";
		}
	}

	for (int i = 0; i < MAX_SCENES; i++)
	{
		if (i < m_SceneManager->m_SceneList.size())
		{
			m_SceneList[i] = m_SceneManager->m_SceneList.at(i)->m_SceneName;
		}
		else
		{
			m_SceneList[i] = " ";
		}
	}

	for (int i = 0; i < MAX_CAMERAS; i++)
	{
		if (i < m_SceneManager->m_SceneList.at(m_SceneManager->GetActiveScene())->GetAssets().m_Cameras.size())
		{
			m_Cameras[i] = m_SceneManager->m_SceneList.at(m_SceneManager->GetActiveScene())->GetAssets().m_Cameras.at(i)->name;
		}
		else
		{
			m_Cameras[i] = " ";
		}
	}

	ImGui_ImplGlfw_NewFrame();
	ImGui_ImplOpenGL3_NewFrame();

	ImGui::NewFrame();

	ImGui::Begin("Inspector");

	//ImGui::Text(m_SceneManager->GetCurrentScene()->GetAssets().m_Objects.at(selected)->name);
	if (m_SceneManager->m_SceneList.at(m_SceneManager->GetActiveScene())->GetAssets().m_Objects.size() != 0)
	{
		ImGui::BeginChild("Transform", ImVec2(0, 200), true);

		ImGui::Checkbox("Active", &m_SceneManager->m_SceneList.at(m_SceneManager->GetActiveScene())->GetAssets().m_Objects.at(selected)->m_Active);
		ImGui::Text("Transform");
		ImGui::InputFloat2("Position", &m_SceneManager->m_SceneList.at(m_SceneManager->GetActiveScene())->GetAssets().m_Objects.at(selected)->transform.position.x);
		ImGui::InputFloat("Rotation", &m_SceneManager->m_SceneList.at(m_SceneManager->GetActiveScene())->GetAssets().m_Objects.at(selected)->transform.rotation);
		ImGui::InputFloat2("Size", &m_SceneManager->m_SceneList.at(m_SceneManager->GetActiveScene())->GetAssets().m_Objects.at(selected)->transform.size.x);
		ImGui::EndChild();
	}

	if (m_SceneManager->m_SceneList.at(m_SceneManager->GetActiveScene())->GetAssets().m_Cameras.size() != 0)
	{
		ImGui::BeginChild("Camera Transform", ImVec2(0, 200), true);
		ImGui::Text("Camera Transform");
		ImGui::InputFloat2("Position", &m_SceneManager->m_SceneList.at(m_SceneManager->GetActiveScene())->GetAssets().m_Cameras.at(Cameraselected)->transform.position.x);
		ImGui::InputFloat("Rotation", &m_SceneManager->m_SceneList.at(m_SceneManager->GetActiveScene())->GetAssets().m_Cameras.at(Cameraselected)->transform.rotation);
		ImGui::Text(" ");
		ImGui::Text("Camera Lens");
		ImGui::InputFloat("Zoom", &m_SceneManager->m_SceneList.at(m_SceneManager->GetActiveScene())->GetAssets().m_Cameras.at(Cameraselected)->zoom);
		ImGui::InputFloat("Far", &m_SceneManager->m_SceneList.at(m_SceneManager->GetActiveScene())->GetAssets().m_Cameras.at(Cameraselected)->far);
		ImGui::InputFloat("Near", &m_SceneManager->m_SceneList.at(m_SceneManager->GetActiveScene())->GetAssets().m_Cameras.at(Cameraselected)->near);
		ImGui::EndChild();
	}
	ImGui::BeginMainMenuBar();
	ImGui::Text(PROJECT_NAME);
	ImGui::Button("Save Current");
	ImGui::Button("Project Settings");

	if (ImGui::Button("Help"))
	{
		ShowHelp = true;
	}

	if (ImGui::ArrowButton("Play", ImGuiDir_Right) && m_Mode == EDITOR)
	{
		m_Mode = EDITOR_PLAY;
		m_SceneManager->m_SceneList.at(m_SceneManager->GetActiveScene())->Start();
		UpdateEditorMode();
	}
	if (ImGui::Button("||"))
	{
		if (m_Mode == EDITOR_PLAY)
		{
			m_Mode = EDITOR_PAUSED;
		}
		else if(m_Mode == EDITOR_PAUSED)
		{
			m_Mode = EDITOR_PLAY;
		}
	}
	if (ImGui::Button("STOP") && m_Mode == EDITOR_PLAY)
	{
		m_Mode = EDITOR;
		m_SceneManager->m_SceneList.at(m_SceneManager->GetActiveScene())->Start();
		UpdateEditorMode();
	}



	if (ShowHelp)
	{
		ImGui::Begin("Help");
		if (ImGui::Button("close"))
		{
			ShowHelp = false;
		}
		ImGui::Text("Welcome to the Vertex Engine Editor! This editor allows for developers to place objects around a scene.");
		ImGui::Text("The engine can be toggled between Play, Editor Play & Editor mode by changing the mode inside the Application.h file.");
		ImGui::Text("This Editor cannot add or delete GameObjects from a scene. Objects can only be edited & moved around to create a scene.");
		ImGui::Text("All GameObjects must be created in code first & registered to a AssetManager to be shown in the editor.");
		ImGui::End();
	}

	ImGui::EndMainMenuBar();
	ImGui::End();

	//=====================================

	ImGui::Begin("Assets");

	ImGui::Text("Currently Editing: ");
	ImGui::Text(m_SceneList[currentScene]);
	ImGui::Text("Scene Objects");

	ImGui::ListBox("Assets", &selected, m_Assets, m_SceneManager->GetCurrentScene()->GetAssets().m_Objects.size());
	ImGui::ListBox("Cameras", &Cameraselected, m_Cameras, m_SceneManager->GetCurrentScene()->GetAssets().m_Cameras.size());

	ImGui::End();

	if (glfwGetKey(m_GameWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS && m_Mode == EDITOR_PLAY)
	{
		m_Mode = EDITOR;
	}

	//=====================================


	ImGui::Begin("Project Drawer");

	ImGui::Text("Scenes To Edit");

	if (ImGui::ListBox("##Assets", &currentScene, m_SceneList, m_SceneManager->m_SceneList.size()))
	{
		selected = 0;
		m_SceneManager->SetActiveScene(currentScene);
		m_SceneManager->PrintActiveScene();
	}

	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Application::RenderAll()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_SceneManager->RenderCurrentScene(m_Renderer);
	if (m_Mode == EDITOR || m_Mode == EDITOR_PLAY || m_Mode == EDITOR_PAUSED)
	{
		Editor();
	}

	glfwSwapBuffers(m_GameWindow);
	glfwPollEvents();
}

void Application::Quit()
{
	m_IsRunning = false;
}

//=============================================== Add your scenes Here
void Application::SceneSetUp()
{
	m_SceneManager->SetActiveScene(0); // Sets the current scene to the first in the list.

	m_Scene = new MyScene("Scene 1");
	m_SecondScene = new Scene2("Scene 2");

	m_SceneManager->AddScene(m_Scene);
	m_SceneManager->AddScene(m_SecondScene);


	//============================================================ Remove this & automate it in the scene manager! Temp for testing
	m_Scene->GiveWindow(m_GameWindow);
	m_SecondScene->GiveWindow(m_GameWindow);

	m_Scene->GiveSceneManager(m_SceneManager);
	m_SecondScene->GiveSceneManager(m_SceneManager);

	m_SceneManager->PrintActiveScene(); // Delete if you want
}

void Application::UpdateEditorMode()
{
	if (m_Mode == EDITOR)
	{
		ImGuiStyle* style = &ImGui::GetStyle();
		style->Colors[ImGuiCol_WindowBg] = ImVec4(EDITOR_BACKGROUND);
		style->Colors[ImGuiCol_MenuBarBg] = ImVec4(EDITOR_MENU);
		style->Colors[ImGuiCol_TitleBg] = ImVec4(EDITOR_TABS);

		style->Colors[ImGuiCol_TitleBgActive] = ImVec4(EDITOR_TABS_SELECT);
		style->Colors[ImGuiCol_Tab] = ImVec4(EDITOR_TABS_SELECT);
		style->Colors[ImGuiCol_Button] = ImVec4(EDITOR_MENU);
		style->Colors[ImGuiCol_ButtonHovered] = ImVec4(EDITOR_BUTTONS_SELECT);
		std::cout << "Editor Mode" << std::endl;

	}
	else if(m_Mode == EDITOR_PLAY)
	{
		ImGuiStyle* style1 = &ImGui::GetStyle();
		style1->Colors[ImGuiCol_WindowBg] = ImVec4(PLAY_MODE_COLOUR);
		style1->Colors[ImGuiCol_MenuBarBg] = ImVec4(PLAY_MODE_COLOUR);
		style1->Colors[ImGuiCol_TitleBg] = ImVec4(PLAY_MODE_COLOUR);

		style1->Colors[ImGuiCol_TitleBgActive] = ImVec4(PLAY_MODE_COLOUR);
		style1->Colors[ImGuiCol_Tab] = ImVec4(PLAY_MODE_COLOUR);
		style1->Colors[ImGuiCol_Button] = ImVec4(PLAY_MODE_COLOUR);
		style1->Colors[ImGuiCol_ButtonHovered] = ImVec4(PLAY_MODE_COLOUR);
		std::cout << "Play Mode" << std::endl;
	}
}

void Application::ShutDown()
{
	if (m_Mode == EDITOR || m_Mode == EDITOR_PLAY)
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
	glfwTerminate();
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
}
