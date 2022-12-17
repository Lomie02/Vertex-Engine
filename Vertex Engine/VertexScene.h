#pragma once
#include "VertexRenderPackage.h"
#include "AssetManager.h"
class VertexScene {

public:
	VertexScene();
	VertexScene(const char _name[]);
	virtual void Awake() = 0;
	virtual void Start() = 0;
	virtual void Update(float delta) = 0;

	virtual void LateUpdate(float delta) = 0;
	virtual void FixedUpdate(float delta) = 0;

	virtual void Rendering(Vertex2D* render) = 0;

	virtual void GiveWindow(GLFWwindow* window) { m_Window = window; }
	virtual void GiveSceneManager(SceneManager* scenes) { m_SceneManager = scenes; }
	virtual AssetManager GetAssets() { return m_Manager; }
	char m_SceneName[15] = "MyScene";
protected:
	AssetManager m_Manager;
	SceneManager* m_SceneManager;
	GLFWwindow m_Window;
};