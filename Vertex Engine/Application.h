#pragma once
#include "Graphics.h"
#include "MyScene.h"
#include "Scene2.h"
#include "Vertex2D.h"
#include "SceneManager.h"
#include "VertexText2D.h"

#define SIZE m_Size

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
	void Editor();

	GLFWwindow* GetWindow() { return m_GameWindow; }

	void RenderAll();
	void Quit();

	bool AppStatus() { return m_IsRunning; };
	void ShutDown();

private:

	void FolderCreation();
	void SceneSetUp();
	void UpdateEditorMode();

	EditorMode m_Mode = EDITOR;
	GLFWwindow* m_GameWindow = nullptr;

	bool m_IsRunning = false;

	float m_TimeStep = 1.0f;
	VertexText2D* Text;
	Vertex2D* m_Renderer;
	SceneManager* m_SceneManager;

	MyScene* m_Scene;
	Scene2* m_SecondScene;

	bool m_EditorFullScreen = false;

	int m_Size =0;
	float m_fps = 0;
	float m_frames = 0;
	float m_deltaTime = 0;
	float m_fpsInterval = 0;
	float m_prevTime;
};