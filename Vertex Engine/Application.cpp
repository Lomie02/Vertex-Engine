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

	m_ApplicationFramesPerSecond = 0;
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
	m_Settings->m_AutoDeletePointers = AUTO_DELETE_ASSET_POINTERS;
	m_Settings->m_TransparentSortingAlgo = Insertion_Sort;

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
	if (USE_VERTEX_WORK_SPACE) {
		m_WorkSpaceEditor.SceneCreation();
		SetUpWorkSpaceScenes(); // Set Up scenes from the workspace if being used.
	}
	else {
		SceneSetUp(); //Start setting up all scenes in engine.
	}

	BeginSceneSystemAssigning();


	for (int i = 0; i < m_SceneManager->m_SceneList.size(); i++) {
		m_SceneManager->m_SceneList.at(i)->GetAssets().AssignSoundSystem(m_SoundManager);
	}

	switch (m_Mode) {
	case PLAY:
		m_SceneManager->SetActiveScene(0);
		break;

	case EDITOR:
		m_SceneManager->SetActiveScene(1);
		break;
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
			m_ApplicationFramesPerSecond = m_frames;
			m_frames = 0;
			m_fpsInterval -= 1.0f;
		}


		static float fixedDelta = 0.0f;
		fixedDelta += m_deltaTime;

		if (m_FinishedSceneSetUpStage)
			m_SceneManager->UpdateScenes(m_deltaTime); // Update regular loop

		while (fixedDelta >= m_TimeStep)
		{
			m_SceneManager->UpdateFixedScenes(m_TimeStep); // Add timestep to Fixed & Late Update Loop.
			m_SceneManager->GetCurrentScene()->GetAssets().ConfigurePhysics(m_TimeStep);
			fixedDelta -= m_TimeStep;
		}

		m_SceneManager->GetCurrentScene()->GetAssets().ConfigureSystems(); // Update asset managers systems
	}
}

void Application::SetUpWorkSpaceScenes()
{
	for (int i = 0; i < m_WorkSpaceEditor.GrabWorkSpaceScenes().size(); i++) {

		m_SceneManager->AddScene(m_WorkSpaceEditor.GrabWorkSpaceScenes().at(i));
	}
}

void Application::EditorMain() // Main Editor
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
		case eSprite:
			if (ImGui::TreeNode("Transform"))
			{
				ImGui::Text(m_SceneManager->m_SceneList.at(m_SceneManager->GetActiveScene())->GetAssets().m_Objects.at(selectedSprite)->name);
				ImGui::SameLine(); ImGui::Text(" Properties");
				ImGui::Checkbox("Active", &m_SceneManager->m_SceneList.at(m_SceneManager->GetActiveScene())->GetAssets().m_Objects.at(selectedSprite)->m_Active);
				ImGui::InputInt("Layer", &m_SceneManager->m_SceneList.at(m_SceneManager->GetActiveScene())->GetAssets().m_Objects.at(selectedSprite)->layer, 2);

				ImGui::Spacing();
				ImGui::Image((void*)(intptr_t)m_SceneManager->m_SceneList.at(m_SceneManager->GetActiveScene())->GetAssets().m_Cameras.at(1)->renderTexture->GetTexture(),ImVec2(640,360), ImVec2(0,1), ImVec2(1,0));

				//===================================== Position

				if (ImGui::Button("X")) {
					m_SceneManager->m_SceneList.at(m_SceneManager->GetActiveScene())->GetAssets().m_Objects.at(selectedSprite)->transform.position.x = 0;
				};
				ImGui::SameLine();

				ImGui::InputFloat("##Xpos", &m_SceneManager->m_SceneList.at(m_SceneManager->GetActiveScene())->GetAssets().m_Objects.at(selectedSprite)->transform.position.x, 0.0f, 0.0f, "%.1f");

				if (ImGui::Button("Y")) {
					m_SceneManager->m_SceneList.at(m_SceneManager->GetActiveScene())->GetAssets().m_Objects.at(selectedSprite)->transform.position.y = 0;
				}
				//ImGui::PopStyleColor();

				ImGui::SameLine(); ImGui::InputFloat("##Ypos", &m_SceneManager->m_SceneList.at(m_SceneManager->GetActiveScene())->GetAssets().m_Objects.at(selectedSprite)->transform.position.y);

				EditorSpacer(2);
				//===================================== Rotation
				ImGui::InputFloat("Rotation", &m_SceneManager->m_SceneList.at(m_SceneManager->GetActiveScene())->GetAssets().m_Objects.at(selectedSprite)->transform.rotation);

				//===================================== Scale
				ImGui::InputFloat("Scale", &m_SceneManager->m_SceneList.at(m_SceneManager->GetActiveScene())->GetAssets().m_Objects.at(selectedSprite)->transform.scale);
				ImGui::InputFloat2("Size", &m_SceneManager->m_SceneList.at(m_SceneManager->GetActiveScene())->GetAssets().m_Objects.at(selectedSprite)->transform.size.x);
				ImGui::Spacing();

				ImGui::Text("Pivots & Centers");
				ImGui::InputFloat2("Pivot", &m_SceneManager->m_SceneList.at(m_SceneManager->GetActiveScene())->GetAssets().m_Objects.at(selectedSprite)->transform.pivot.x);

				ImGui::TreePop();

				EditorSpacer(2);
			}

			if (ImGui::TreeNode("Material")) {
				//=========================== Surface Type
				EditorSpacer(2);
				static const char* surface[2];
				surface[0] = "Opaque";
				surface[1] = "Transparent";

				static const char* blendmode[3];
				blendmode[0] = "Alpha";
				blendmode[1] = "Additive";
				blendmode[2] = "Screen";

				static int surfaceType;
				static int blendModeValue;

				surfaceType = m_SceneManager->m_SceneList.at(m_SceneManager->GetActiveScene())->GetAssets().m_Objects.at(selectedSprite)->material.surface;
				blendModeValue = m_SceneManager->m_SceneList.at(m_SceneManager->GetActiveScene())->GetAssets().m_Objects.at(selectedSprite)->material.TransparencyBlend;
				if (ImGui::Combo("Surface Type", &surfaceType, surface, 2)) {
					if (surfaceType == 0) {
						m_SceneManager->m_SceneList.at(m_SceneManager->GetActiveScene())->GetAssets().m_Objects.at(selectedSprite)->material.surface = Opaque;
					}
					else {

						m_SceneManager->m_SceneList.at(m_SceneManager->GetActiveScene())->GetAssets().m_Objects.at(selectedSprite)->material.surface = Transparent;
					}
				};

				EditorSpacer(2);

				// Setting blending mode in editor
				if (ImGui::Combo("Blend Mode", &blendModeValue, blendmode, 3)) {
					if (blendModeValue == 0) {
						m_SceneManager->m_SceneList.at(m_SceneManager->GetActiveScene())->GetAssets().m_Objects.at(selectedSprite)->material.TransparencyBlend = Alpha;
					}
					else if (blendModeValue == 1) {

						m_SceneManager->m_SceneList.at(m_SceneManager->GetActiveScene())->GetAssets().m_Objects.at(selectedSprite)->material.TransparencyBlend = Additive;
					}
					else {
						m_SceneManager->m_SceneList.at(m_SceneManager->GetActiveScene())->GetAssets().m_Objects.at(selectedSprite)->material.TransparencyBlend = Screen;
					}
				};

				//=========================== Basic Colour
				ImGui::DragFloat4("Colour", &m_SceneManager->m_SceneList.at(m_SceneManager->GetActiveScene())->GetAssets().m_Objects.at(selectedSprite)->material.colour.r, 0.05f, 0.0f, 1.0f);
				ImGui::TreePop();
			}

			break;

		case eCamera:

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
				ImGui::InputFloat("Far", &m_SceneManager->m_SceneList.at(m_SceneManager->GetActiveScene())->GetAssets().m_Cameras.at(selectedCamera)->farClip);
				ImGui::InputFloat("Near", &m_SceneManager->m_SceneList.at(m_SceneManager->GetActiveScene())->GetAssets().m_Cameras.at(selectedCamera)->nearClip);
				ImGui::EndChild();
			}
			break;
		case eGuiText: // Text Display in the editor

			ImGui::Text(m_SceneManager->m_SceneList.at(m_SceneManager->GetActiveScene())->GetAssets().GetTextObjects().at(selectedTextInterface)->name);
			ImGui::SameLine(); ImGui::Text("Properties");
			ImGui::Checkbox("Active", &m_SceneManager->m_SceneList.at(m_SceneManager->GetActiveScene())->GetAssets().GetTextObjects().at(selectedTextInterface)->m_Active);

			if (ImGui::TreeNode("Transform")) {

				ImGui::Text("Position");
				ImGui::Button("X"); ImGui::SameLine();

				ImGui::InputFloat("##Xpos", &m_SceneManager->m_SceneList.at(m_SceneManager->GetActiveScene())->GetAssets().GetTextObjects().at(selectedTextInterface)->transform.position.x, 0.0f, 0.0f, "%.1f");

				ImGui::Button("Y");

				ImGui::SameLine(); ImGui::InputFloat("##Ypos", &m_SceneManager->m_SceneList.at(m_SceneManager->GetActiveScene())->GetAssets().GetTextObjects().at(selectedTextInterface)->transform.position.y);

				ImGui::Spacing();
				ImGui::Spacing();
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Material")) {

				ImGui::DragFloat4("Colour", &m_SceneManager->m_SceneList.at(m_SceneManager->GetActiveScene())->GetAssets().GetTextObjects().at(selectedTextInterface)->material.colour.r, 0.05f, 0.0f, 1.0f);
				ImGui::TreePop();
			}
			break;
		}
	}

	// Camera Transform UI

	ImGui::End();

	ImGui::BeginMainMenuBar();
	ImGui::Text(PROJECT_NAME);
	ImGui::Spacing();

	if (ImGui::Button("Layout"))
	{
		m_WindowMode = Main;
	};

	ImGui::Spacing();

	if (ImGui::Button("Flipbook"))
	{
		m_WindowMode = Animation;
	};
	static bool ViewInfo;
	if (ImGui::Button("Information"))
	{
		VertexAccessExplorer::OpenURL(L"https://www.google.com/search?sca_esv=561834441&rlz=1C1GCEA_enAU1052AU1052&q=winton+overwat&tbm=isch&source=lnms&sa=X&ved=2ahUKEwj4-oe10YiBAxXGamwGHawoC-0Q0pQJegQIDRAB&biw=1920&bih=963&dpr=1");
		ViewInfo = true;
	};

	ImGui::Spacing();

	ImGui::Button("");

	ImGui::Spacing();

	for (int i = 0; i < 68; i++) //Space out the play buttons
	{
		ImGui::Spacing();
	}

	// Focusing Objects

	if (glfwGetKey(m_GameWindow, GLFW_KEY_F) == GLFW_PRESS)
	{
		switch (m_EditorSelectType) {
		case eSprite:
			if (m_SceneManager->GetCurrentScene()->GetAssets().m_Objects.size() != 0) {
				m_SceneManager->GetCurrentScene()->GetAssets().m_Cameras.at(0)->transform.position = m_SceneManager->GetCurrentScene()->GetAssets().m_Objects.at(selectedSprite)->transform.position;
			}
			break;
		case eCamera:
			m_SceneManager->GetCurrentScene()->GetAssets().m_Cameras.at(0)->transform.position = m_SceneManager->GetCurrentScene()->GetAssets().m_Cameras.at(selectedCamera)->transform.position;
			break;

		case eGUI:
			m_SceneManager->GetCurrentScene()->GetAssets().m_Cameras.at(0)->transform.position = m_SceneManager->GetCurrentScene()->GetAssets().GetButtonObjects().at(selectedUserInterface)->transform.position;
			break;
		}
	}

	if (ViewInfo) { // Information Window
		ImGui::Begin("Vertex Information");


		if (ImGui::TreeNode("Vertex Main Systems")) {

			ImGui::Button("GameObjects");
			ImGui::Button("Physics Object");
			ImGui::Button("Sound & Audio");
			ImGui::Button("Asset Manager");
			ImGui::Button("Canvas");
			ImGui::Button("UI Button");
			ImGui::Button("Text Objects");

			ImGui::Spacing();
			ImGui::Button("Vertex2D");
			ImGui::Button("Tension2D");
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Vertex Components")) {

			ImGui::Button("Mimes");
			ImGui::Button("FlipBook Animation");
			ImGui::Button("Animator");
			ImGui::Button("Player Controller");
			ImGui::Button("FlipClips");
			ImGui::Button("Clips");

			ImGui::TreePop();
		}

		if (ImGui::Button("Exit"))
			ViewInfo = false;
		ImGui::End();
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
			m_EditorSelectType = eSprite;
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
			if (ImGui::Button(m_SceneManager->GetCurrentScene()->GetAssets().m_Objects.at(i)->name))
			{
				selectedSprite = i;
				m_EditorSelectType = eSprite;
			}

		}

		// Button Tree
		for (int i = 0; i < m_SceneManager->GetCurrentScene()->GetAssets().GetButtonObjects().size(); i++) {

			ImGui::Spacing();
			if (ImGui::Button(m_SceneManager->GetCurrentScene()->GetAssets().GetButtonObjects().at(i)->name))
			{
				selectedUserInterface = i;
				m_EditorSelectType = eGUI;

			}
		}
		// Camera Tree
		for (int i = 0; i < m_SceneManager->GetCurrentScene()->GetAssets().m_Cameras.size(); i++) {

			ImGui::Spacing();
			if (ImGui::Button(m_SceneManager->GetCurrentScene()->GetAssets().m_Cameras.at(i)->name))
			{
				m_EditorSelectType = eCamera;
				selectedCamera = i;
			}
		}

		// Text Tree
		for (int i = 0; i < m_SceneManager->GetCurrentScene()->GetAssets().GetTextObjects().size(); i++) {

			ImGui::Spacing();
			if (ImGui::Button(m_SceneManager->GetCurrentScene()->GetAssets().GetTextObjects().at(i)->name))
			{
				m_EditorSelectType = eGuiText;
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

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Application::EditorAnimation()
{
}

void Application::EditorHud()
{
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

	for (int i = 0; i < m_SceneManager->m_SceneList.size(); i++)
	{
		m_SceneManager->m_SceneList.at(i)->GetAssets().AssignMode(m_Mode);
		m_SceneManager->m_SceneList.at(i)->GetAssets().GiveWindow(m_GameWindow);
		m_SceneManager->m_SceneList.at(i)->GetAssets().GiveSceneManager(m_SceneManager);
	}

	m_FinishedSceneSetUpStage = true;
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

/// <summary>
///  Updates the interface for the editor.
/// </summary>
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
	ResourceManager::LoadTexture("Builds/Textures/UI_Button.png", "UI_Button");
}
