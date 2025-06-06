#pragma once
#include "Graphics.h"
#include "MyScene.h"
#include "Vertex2D.h"

#include "VertexTransitions.h"
#include "SceneManager.h"
#include "VertexText2D.h"
#include "AdvancedSettings.h"

#include "BootUpContainer.h"
#include "VertexGeneral.h"
#include "MyFirstScene.h"
#include <irrKlang.h>

#include <chrono>
#include "VertexWorkSpace.h"
#include "VertexEngineEditor.h"
#include "AssetPipelineManager.h"
#include "Debug.h" 

using namespace irrklang;
using Clock = std::chrono::high_resolution_clock;
/// <summary>
/// The editor window that should be displayed.
/// </summary>
enum EditorWindow {
	Main = 0,
	Animation,
	HudEditor,
};

//TODO: Find out if this is needed or not. Otherwise delete it.
enum EditorSelection {
	eSprite = 0,
	eCamera,
	eGUI,
	eGuiText,
};

/// <summary>
/// The Anti Alias mode used for renderering.
/// </summary>
enum AntiAliasingMode {
	none = 0,
	MSAA,
	SMAA,
};
class Application
{
public:
	//Default Constructor
	Application();
	//De-Constructor
	~Application();
	/// <summary>
	/// Is called at the very start of the engines creation and sets up all major systems and passes them into their respective classes.
	/// </summary>
	void StartUp();
	/// <summary>
	/// The Engines start function calls all other systems start ups and gets them ready.
	/// </summary>
	void Start();
	/// <summary>
	/// Is called everyframe and is responsible for updating the engines systems & calulates the delta time values.
	/// </summary>
	void Update();
	/// <summary>
	/// Gets a pointer to the current OpenGL Window.
	/// </summary>
	/// <returns>Pointer to Window.</returns>
	GLFWwindow* GetWindow() { return m_GameWindow; }
	/// <summary>
	/// Updates the current scene thats active & passes in the renderer being used. Also draws all the UI for the editor.
	/// </summary>
	void RenderAll();
	/// <summary>
	/// Calls the shutdown method and deletes all of the systems.
	/// </summary>
	void Quit();
	/// <summary>
	/// Returns if the engine is running or has closed.
	/// </summary>
	/// <returns></returns>
	bool AppStatus() { return m_IsRunning; };
	/// <summary>
	/// Starts the shutdown process & deletes all systems correctly.
	/// </summary>
	void ShutDown();

	EditorMode GetCurrentEditorMode() { return m_Mode; }
	void SetEditorMode(EditorMode _mode) { m_Mode = _mode; };

	// These functions will take care of the external packages that the engine will soon support.
	void StartExternal();
	void UpdateExternal();

private:
	VertexEngineEditor* m_VertexEditor = nullptr;

	void SetUpWorkSpaceScenes();

	// Editor
	void EditorMain();
	void EditorAnimation();
	void OpenEditor();
	void EditorHud();
	void EditorSpacer(int _spaces);

	void BeginSceneSystemAssigning();

	EditorSelection m_EditorSelectType = eSprite;

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
	Scene2* m_Scene;
	MyFirstScene* m_MyScene;

	AntiAliasingMode m_AnitAliasing = none;
	VertexWorkSpace m_WorkSpaceEditor;

	//Delta Time 
	/*float m_ApplicationFramesPerSecond = 0;
	float m_frames = 0;
	float m_UnScaledDelta = 0;

	float m_fpsInterval = 0;
	float m_prevTime;
	float m_TimeStep = 0.01f;
	float m_TimeScale;*/

	// Fps
	int m_FramesPassed;
	float m_FrameRateCounter;
	int m_CurrentFramesPerSecond;

};