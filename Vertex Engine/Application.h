#pragma once
#include "Graphics.h"
#include "MyScene.h"
#include "Scene2.h"
#include "Vertex2D.h"

#include "SceneManager.h"
#include "VertexText2D.h"
#include "AdvancedSettings.h"
#include "BootUpContainer.h"

#include "MyFirstScene.h"

#define SIZE m_Size

enum EditorWindow {
	Main = 0,
	Animation,
	HudEditor,
};

enum EditorMode {
	EDITOR = 0,
	PLAY,
	EDITOR_PLAY,
	EDITOR_PAUSED,
};

class Application
{
public:
	Application();
	~Application();

	void StartUp();
	void Start();

	void Update();

	GLFWwindow* GetWindow() { return m_GameWindow; }

	void RenderAll();
	void Quit();

	bool AppStatus() { return m_IsRunning; };
	void ShutDown();

	void C_LogComment(std::string _log);
	void C_LogError(std::string _log);
	void C_LogWarning(std::string _log);

private:

	void EditorMenu(EditorWindow _window);
	void EditorSpacer(int _amount = 1);

	void EditorMain();
	void EditorAnimation();
	void EditorHud();
	// Editors
	void FolderCreation();
	void SceneSetUp();
	void UpdateEditorMode();

	std::vector<std::string> m_ConsoleLogs;

	EditorWindow m_WindowMode = Main;
	EditorMode m_Mode = EDITOR;
	GLFWwindow* m_GameWindow = nullptr;

	static const char* m_ConsoleLog[50];
	bool m_IsRunning = false;

	VertexText2D* Text;
	Vertex2D* m_Renderer;
	SceneManager* m_SceneManager;

	MyFirstScene* m_MyScene;

	MyScene* m_Scene;
	Scene2* m_SecondScene;

	bool m_EditorFullScreen = false;
	int m_Size = 0;

	//Time
	float m_fps = 0;
	float m_frames = 0;
	float m_deltaTime = 0;
	float m_fpsInterval = 0;
	float m_prevTime;

	BootUpContainer* m_Settings;

	//Physics
	float m_TimeStep = 0.01f;

	// The renderer that the engine will use.
	Renderer m_UsingRenderer = Tension_2D;
};