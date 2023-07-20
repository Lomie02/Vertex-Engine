#pragma once
#include "Graphics.h"
#include "MyScene.h"
#include "Scene2.h"
#include "Vertex2D.h"

#include "VertexTransitions.h"
#include "SceneManager.h"
#include "VertexText2D.h"
#include "AdvancedSettings.h"

#include "BootUpContainer.h"
#include "VertexGeneral.h"
#include "MyFirstScene.h"
#include <irrKlang.h>

using namespace irrklang;

//Determines what editor window to display.
enum EditorWindow {
	Main = 0,
	Animation,
	HudEditor,
};

class Application
{
public:

	//Default Constructor
	Application();

	//De-Constructor
	~Application();

	//Creates all major systems & passes them through their respective classes.
	void StartUp();

	// Calls all start functions in scenes & scripts.
	void Start();

	// Update basic scenes like Update(),FixedUpdate() & LateUpdate()
	void Update();

	// Returns the opengl window.
	GLFWwindow* GetWindow() { return m_GameWindow; }

	// Updates all renderering in the engine such as game scenes to editors.
	void RenderAll();

	// Quits the engine
	void Quit();

	//Is the app running or not.
	bool AppStatus() { return m_IsRunning; };

	//Shutdown the app & Delete all systems if any.
	void ShutDown();

	// These functions will take care of the external packages that the engine will soon support.
	void StartExternal();
	void UpdateExternal();

private:

	// Editor
	void EditorMain();
	void EditorAnimation();
	void EditorHud();

	EditorWindow m_WindowMode = Main;
	EditorMode m_Mode = EDITOR;
	bool m_EditorFullScreen = false;
	
	// Basics
	void FolderCreation();
	void ExternalResources();
	void SceneSetUp();
	void UpdateEditorMode();

	bool m_IsRunning = false;
	GLFWwindow* m_GameWindow = nullptr;

	//Engine Majors
	VertexTransitions* m_TransitionScene;
	VertexText2D* Text;
	Vertex2D* m_Renderer;
	SceneManager* m_SceneManager;

	Renderer m_UsingRenderer = Tension_2D;
	BootUpContainer* m_Settings;
	ISoundEngine* m_SoundManager;
	bool m_FinishedSceneSetUpStage = false;

	//Vertex Scenes
	MyScene* m_Scene;
	Scene2* m_SecondScene;
	MyFirstScene* m_MyScene;

	//Delta Time 
	float m_fps = 0;
	float m_frames = 0;
	float m_deltaTime = 0;

	float m_fpsInterval = 0;
	float m_prevTime;
	float m_TimeStep = 0.01f;
};