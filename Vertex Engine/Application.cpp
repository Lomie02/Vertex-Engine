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

#define GL_SILENCE_DEPRECATION
#pragma warning(disable : 4996);

//==========================================
Application::Application()
{
	m_SoundManager = createIrrKlangDevice();
}

Application::~Application()
{
	delete m_SceneManager;
	m_SceneManager = nullptr;

	delete m_Renderer;

	delete m_Scene;
	m_Scene = nullptr;

	delete m_Settings;
	m_Settings = nullptr;

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

	if (FINAL_BUILD)
	{
		m_Mode = PLAY;
	}

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

	char other1[20] = PROJECT_ICON;
	char name1[100] = "Builds/Textures/Icons/";

	strcat(name1, other1);
	Texture2D data;

	int width;
	int height;

	int channels;
	unsigned char* picture;

	if (m_Mode == PLAY)
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

	m_fps = 0;
	m_frames = 0;
	m_deltaTime = 0;
	m_fpsInterval = 0;
	m_prevTime = (float)glfwGetTime();

	FolderCreation();

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


	if (m_Mode == EDITOR)
	{
		m_EditorFullScreen = false;
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		ImGui_ImplGlfw_InitForOpenGL(m_GameWindow, true);
		ImGui_ImplOpenGL3_Init("#version 330");

		ImGui::StyleColorsDark();

		//Commented out to test a new editor desigh

		//glViewport(299.973f, 349.968f, 1280, 720);
		//glEnable(GL_SCISSOR_TEST);

		UpdateEditorMode();
		std::cout << "Vertex Message: Editor Succeded." << std::endl;
	}

	for (int i = 0; i < m_SceneManager->m_SceneList.size(); i++)
	{
		m_SceneManager->m_SceneList.at(i)->GetAssets().BootUpAll(m_Settings);
	}

	std::cout << "Vertex Message: Start Up Succeded." << std::endl;

	ExternalResources(); //Load External files.

	m_SceneManager->EngineState(m_Mode);

	glClearColor(BACKGROUND_COLOUR);
}

void Application::Start()
{
	SceneSetUp(); //Start setting up all scenes in engine.

	if (m_Mode != PLAY) {
		m_SceneManager->SetActiveScene(1);
	}
	else {
		m_SceneManager->SetActiveScene(0);
	}

	m_SceneManager->StartUpScenes();

	std::cout << "Vertex Message: Start Succeded." << std::endl;
}

void Application::Update()
{
	if (m_Mode == PLAY || m_Mode == EDITOR_PLAY) {

		double currTime = (float)glfwGetTime();
		m_deltaTime = currTime - m_prevTime;
		m_prevTime = currTime;

		if (m_Mode == EDITOR_PLAY && Input::GetKeyDown(m_GameWindow, GLFW_KEY_ESCAPE))
		{
			m_Mode = EDITOR_PLAY;
			Cursor::Show(m_GameWindow);
		}

		// Calulate delta time
		m_frames++;
		m_fpsInterval += m_deltaTime;
		if (m_fpsInterval >= 1.0f)
		{
			m_fps = m_frames;
			m_frames = 0;
			m_fpsInterval -= 1.0f;
		}

		m_SceneManager->GetCurrentScene()->GetAssets().LogEvents(); // Log Positions for collsion


		static float fixedDelta = 0.0f;
		fixedDelta += m_deltaTime;

		if (m_FinishedSceneSetUpStage)
			m_SceneManager->GetCurrentScene()->GetAssets().ConfigureSystems(); // Update asset managers systems

		m_SceneManager->UpdateScenes(m_deltaTime); // Update regular loop

		while (fixedDelta >= m_TimeStep)
		{
			m_SceneManager->UpdateFixedScenes(m_TimeStep); // Add timestep to Fixed & Late Update Loop.
			m_SceneManager->GetCurrentScene()->GetAssets().ConfigurePhysics(m_TimeStep);
			fixedDelta -= m_TimeStep;
		}
	}
}

void Application::EditorMain()
{
	static bool DisplaySceneDrawer;
	static const char* m_SceneList[MAX_SCENES];

	static int selectedCamera = 0;
	static int selectedSprite = 0;

	static int selectedUserInterface = 0;
	static int selectedTextInterface = 0;

	static int currentScene = 0;
	static float ScaleAmount = 0;

	//============================================================ TODO: remove this since it is no longer needed anymore.

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

	//==========================================

	ImGui_ImplGlfw_NewFrame();
	ImGui_ImplOpenGL3_NewFrame();

	ImGui::NewFrame();

	ImGui::Begin("Inspector");

	//ImGui::Text(m_SceneManager->GetCurrentScene()->GetAssets().m_Objects.at(selected)->name);
	if (m_SceneManager->m_SceneList.at(m_SceneManager->GetActiveScene())->GetAssets().m_Objects.size() != 0)
	{
		switch (m_EditorSelectType) {
		case Sprite:

			ImGui::Text("Transform");
			ImGui::SameLine();
			ImGui::Spacing();
			ImGui::SameLine();
			ImGui::Spacing();
			ImGui::SameLine();
			ImGui::Checkbox("Active", &m_SceneManager->m_SceneList.at(m_SceneManager->GetActiveScene())->GetAssets().m_Objects.at(selectedSprite)->m_Active);
			ImGui::SameLine(); ImGui::InputInt("Layer", &m_SceneManager->m_SceneList.at(m_SceneManager->GetActiveScene())->GetAssets().m_Objects.at(selectedSprite)->layer, 2);

			//===================================== Position

			ImGui::Text("Position");
			ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(200, 0, 0, 255));
			ImGui::Button("X"); ImGui::SameLine();
			ImGui::PopStyleColor();

			ImGui::InputFloat("##Xpos", &m_SceneManager->m_SceneList.at(m_SceneManager->GetActiveScene())->GetAssets().m_Objects.at(selectedSprite)->transform.position.x, 0.0f, 0.0f, "%.1f");

			ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(0, 200, 0, 255));
			ImGui::Button("Y");
			ImGui::PopStyleColor();

			ImGui::SameLine(); ImGui::InputFloat("##Ypos", &m_SceneManager->m_SceneList.at(m_SceneManager->GetActiveScene())->GetAssets().m_Objects.at(selectedSprite)->transform.position.y);

			ImGui::Spacing();
			ImGui::Spacing();
			//===================================== Rotation
			ImGui::InputFloat("Rotation", &m_SceneManager->m_SceneList.at(m_SceneManager->GetActiveScene())->GetAssets().m_Objects.at(selectedSprite)->transform.rotation);

			//===================================== Scale
			ImGui::InputFloat("Scale", &m_SceneManager->m_SceneList.at(m_SceneManager->GetActiveScene())->GetAssets().m_Objects.at(selectedSprite)->transform.scale);
			ImGui::InputFloat2("Size", &m_SceneManager->m_SceneList.at(m_SceneManager->GetActiveScene())->GetAssets().m_Objects.at(selectedSprite)->transform.size.x);

			ImGui::Spacing();
			ImGui::Spacing();
			//==================================== Colour

			ImGui::Text("Colour");
			ImGui::DragFloat4("##Colour", &m_SceneManager->m_SceneList.at(m_SceneManager->GetActiveScene())->GetAssets().m_Objects.at(selectedSprite)->material.colour.r, 0.05f, 0.0f, 1.0f);

			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Spacing();
			break;

		case Camera:

			if (m_SceneManager->m_SceneList.at(m_SceneManager->GetActiveScene())->GetAssets().m_Cameras.size() != 0)
			{
				ImGui::BeginChild("Camera Transform", ImVec2(0, 200), true);
				ImGui::Text("Camera Transform");

				//==============================================
				ImGui::Text("Position");
				ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(200, 0, 0, 255));
				ImGui::Button("X"); ImGui::SameLine();
				ImGui::PopStyleColor();

				ImGui::InputFloat("##Xpos", &m_SceneManager->m_SceneList.at(m_SceneManager->GetActiveScene())->GetAssets().m_Cameras.at(selectedCamera)->transform.position.x, 0.0f, 0.0f, "%.1f");

				ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(0, 200, 0, 255));
				ImGui::Button("Y");
				ImGui::PopStyleColor();

				ImGui::SameLine(); ImGui::InputFloat("##Ypos", &m_SceneManager->m_SceneList.at(m_SceneManager->GetActiveScene())->GetAssets().m_Cameras.at(selectedCamera)->transform.position.y);

				ImGui::Spacing();
				ImGui::Spacing();

				//==============================================
				ImGui::InputFloat("Rotation", &m_SceneManager->m_SceneList.at(m_SceneManager->GetActiveScene())->GetAssets().m_Cameras.at(selectedCamera)->transform.rotation);
				ImGui::Text(" ");
				ImGui::Text("Camera Lens");
				ImGui::InputFloat("Zoom", &m_SceneManager->m_SceneList.at(m_SceneManager->GetActiveScene())->GetAssets().m_Cameras.at(selectedCamera)->zoom);
				ImGui::InputFloat("Far", &m_SceneManager->m_SceneList.at(m_SceneManager->GetActiveScene())->GetAssets().m_Cameras.at(selectedCamera)->far);
				ImGui::InputFloat("Near", &m_SceneManager->m_SceneList.at(m_SceneManager->GetActiveScene())->GetAssets().m_Cameras.at(selectedCamera)->near);
				ImGui::EndChild();
			}

			break;
		}
	}

	// Camera Transform UI

	ImGui::End();

	ImGui::BeginMainMenuBar();
	ImGui::Text(PROJECT_NAME);
	ImGui::Spacing();

	if (ImGui::Button("Main"))
	{
		m_WindowMode = Main;
	};

	ImGui::Spacing();

	if (ImGui::Button("Animation"))
	{
		m_WindowMode = Animation;
	};

	ImGui::Spacing();

	ImGui::Button("Canvas");

	ImGui::Spacing();

	for (int i = 0; i < 68; i++) //Space out the play buttons
	{
		ImGui::Spacing();
	}

	// Focusing Objects

	if (glfwGetKey(m_GameWindow, GLFW_KEY_F) == GLFW_PRESS)
	{
		switch (m_EditorSelectType) {
		case Sprite:
			if (m_SceneManager->GetCurrentScene()->GetAssets().m_Objects.size() != 0) {
				m_SceneManager->GetCurrentScene()->GetAssets().m_Cameras.at(0)->transform.position = m_SceneManager->GetCurrentScene()->GetAssets().m_Objects.at(selectedSprite)->transform.position;
			}
			break;
		case Camera:
			m_SceneManager->GetCurrentScene()->GetAssets().m_Cameras.at(0)->transform.position = m_SceneManager->GetCurrentScene()->GetAssets().m_Cameras.at(selectedCamera)->transform.position;
			break;

		case GUI:
			m_SceneManager->GetCurrentScene()->GetAssets().m_Cameras.at(0)->transform.position = m_SceneManager->GetCurrentScene()->GetAssets().GetButtonObjects().at(selectedUserInterface)->transform.position;
			break;
		}
	}

	//=======================================================

	if (ImGui::ArrowButton("PLAY", ImGuiDir_Right) && m_Mode == EDITOR)
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
		else if (m_Mode == EDITOR_PAUSED)
		{
			m_Mode = EDITOR_PLAY;
		}
	}
	if (ImGui::Button("STOP"))
	{
		if (m_Mode == EDITOR_PLAY || m_Mode == EDITOR_PAUSED) {

			m_Mode = EDITOR;
			m_SceneManager->m_SceneList.at(m_SceneManager->GetActiveScene())->Start();
			UpdateEditorMode();
		}
	}

	// User Interface Playbuttons.

	for (int i = 0; i < 57; i++) //Space out the play buttons
	{
		ImGui::Spacing();
	}
	if (m_WindowMode == Animation) {

		if (ImGui::ArrowButton("PlayAnimation", ImGuiDir_Right) && m_Mode == EDITOR_PLAY)
		{
			m_SceneManager->GetCurrentScene()->GetAssets().m_Animators.at(0)->Play();
		}

		if (ImGui::Button("KeyFrame") && m_Mode == EDITOR_PLAY)
		{
			m_SceneManager->GetCurrentScene()->GetAssets().m_Animators.at(0)->AddKeyFrame();
		}
		if (ImGui::Button("Stop") && m_Mode == EDITOR_PLAY)
		{
			m_SceneManager->GetCurrentScene()->GetAssets().m_Animators.at(0)->Stop();
		}
	}

	//======================================== Scene Drawer TODO: either get rid of this menu or replace it or have it display only when bot toggling the open editor.
	if (ImGui::Combo("Your Assets", &currentScene, m_SceneList, m_SceneManager->m_SceneList.size(), m_SceneManager->m_SceneList.size()) && m_Mode == EDITOR)
	{
		if (m_Mode == EDITOR)
		{
			selectedSprite = 0;
			m_EditorSelectType = Sprite;
			m_SceneManager->SetActiveScene(currentScene);
			DisplaySceneDrawer = false;
		}
	}

	ImGui::EndMainMenuBar();

	//=====================================
	// Displaying Hirearchy

	ImGui::Begin("Hirearchy");

	if (ImGui::TreeNode(m_SceneList[currentScene]))
	{
		// Sprite Tree
		for (int i = 0; i < m_SceneManager->GetCurrentScene()->GetAssets().m_Objects.size(); i++) {

			ImGui::Spacing();
			if (ImGui::TreeNode(m_SceneManager->GetCurrentScene()->GetAssets().m_Objects.at(i)->name))
			{
				ImGui::TreePop();
			}

			ImGui::SameLine();

			if (ImGui::Button("Edit"))
			{
				selectedSprite = i;
				m_EditorSelectType = Sprite;
			}
		}

		// Button Tree
		for (int i = 0; i < m_SceneManager->GetCurrentScene()->GetAssets().GetButtonObjects().size(); i++) {

			ImGui::Spacing();
			if (ImGui::TreeNode(m_SceneManager->GetCurrentScene()->GetAssets().GetButtonObjects().at(i)->name))
			{

				ImGui::TreePop();
			}
			ImGui::SameLine();
			if (ImGui::SmallButton("Edit")) {

				selectedUserInterface = i;
				m_EditorSelectType = GUI;
			}
		}
		// Camera Tree
		for (int i = 0; i < m_SceneManager->GetCurrentScene()->GetAssets().m_Cameras.size(); i++) {

			ImGui::Spacing();
			if (ImGui::TreeNode(m_SceneManager->GetCurrentScene()->GetAssets().m_Cameras.at(i)->name))
			{
				ImGui::TreePop();
			}

			ImGui::SameLine();

			if (ImGui::Button("Edit")) {

				m_EditorSelectType = Camera;
				selectedCamera = i;
			}
		}

		// Text Tree
		for (int i = 0; i < m_SceneManager->GetCurrentScene()->GetAssets().GetTextObjects().size(); i++) {

			ImGui::Spacing();
			if (ImGui::TreeNode(m_SceneManager->GetCurrentScene()->GetAssets().GetTextObjects().at(i)->name))
			{
				ImGui::TreePop();
			}
			ImGui::SameLine();
			if (ImGui::Button("Edit")) {

				m_EditorSelectType = GuiText;
				selectedTextInterface = i;
			}
		}

		ImGui::TreePop();
	}

	ImGui::End();

	if (glfwGetKey(m_GameWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS && m_Mode == EDITOR_PLAY)
	{
		m_Mode = EDITOR;
	}

	//=====================================

	//ImGui::Begin("Project Drawer");

	//ImGui::Text("Scenes");
	//ImGui::BeginChild("Scenes");

	//if (ImGui::ListBox("##Assets", &currentScene, m_SceneList, m_SceneManager->m_SceneList.size()) && m_Mode == EDITOR)
	//{
	//	if (m_Mode == EDITOR)
	//	{
	//		selectedSprite = 0;
	//		m_EditorSelectType = Sprite;
	//		m_SceneManager->SetActiveScene(currentScene);
	//	}
	//}

	//ImGui::EndChild();

	//ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Application::EditorAnimation()
{
}

void Application::EditorHud()
{
}

void Application::RenderAll()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_SceneManager->GetCurrentScene()->GetAssets().ConfigureRenderSystems(m_Renderer);

	if (m_Mode != PLAY)
	{
		m_SceneManager->GetCurrentScene()->GetAssets().Gizmos(m_Renderer);
	}

	if (m_Mode == EDITOR && !m_EditorFullScreen || m_Mode == EDITOR_PLAY && !m_EditorFullScreen || m_Mode == EDITOR_PAUSED && !m_EditorFullScreen)
	{
		switch (m_WindowMode)
		{
		case Main:
			EditorMain();
			break;

		case Animation:
			EditorAnimation();
			break;

		case HudEditor:
			EditorHud();
			break;
		}
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

	m_Scene = new MyScene("Scene 1");
	m_SecondScene = new Scene2("Scene 2");

	m_SceneManager->AddScene(m_Scene);
	m_SceneManager->AddScene(m_SecondScene);

	//============================================================ Remove this & automate it in the scene manager! Temp for testing

	m_Scene->GiveWindow(m_GameWindow);
	m_SecondScene->GiveWindow(m_GameWindow);

	m_Scene->GiveSceneManager(m_SceneManager);
	m_SecondScene->GiveSceneManager(m_SceneManager);

	m_SceneManager->m_SceneList.at(1)->GetAssets().ConfigSetup();

	for (int i = 0; i < m_SceneManager->m_SceneList.size(); i++)
	{
		m_SceneManager->m_SceneList.at(i)->GetAssets().AssignMode(m_Mode);
		m_SceneManager->m_SceneList.at(i)->GetAssets().GiveWindow(m_GameWindow);
	}

	m_FinishedSceneSetUpStage = true;
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

	}
	else if (m_Mode == EDITOR_PLAY)
	{
		ImGuiStyle* style1 = &ImGui::GetStyle();
		style1->Colors[ImGuiCol_WindowBg] = ImVec4(PLAY_MODE_COLOUR);
		style1->Colors[ImGuiCol_MenuBarBg] = ImVec4(PLAY_MODE_COLOUR);
		style1->Colors[ImGuiCol_TitleBg] = ImVec4(PLAY_MODE_COLOUR);

		style1->Colors[ImGuiCol_TitleBgActive] = ImVec4(PLAY_MODE_COLOUR);
		style1->Colors[ImGuiCol_Tab] = ImVec4(PLAY_MODE_COLOUR);
		style1->Colors[ImGuiCol_Button] = ImVec4(PLAY_MODE_COLOUR);
		style1->Colors[ImGuiCol_ButtonHovered] = ImVec4(PLAY_MODE_COLOUR);
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

	Cursor::Show(m_GameWindow);
	m_SceneManager->GetCurrentScene()->GetAssets().ExecuteAll();
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
	ResourceManager::LoadTexture("Builds/Textures/UI_Button.png", "UI_Button");
}
