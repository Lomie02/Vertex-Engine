#pragma once
#include "VertexRenderPackage.h"
#include "AssetManager.h"

class SceneManager;

class VertexScene {

public:
	VertexScene();
	VertexScene(const char _name[]);
	virtual void Awake() = 0;
	virtual void Start() = 0;
	virtual void Update(float delta) = 0;

	virtual void LateUpdate(float delta) = 0;
	virtual void FixedUpdate(float fixedDelta) = 0;

	virtual void Rendering(Vertex2D* render) = 0;
	void GiveWindow(GLFWwindow* window) { Window = window; }
	void GiveSceneManager(SceneManager* scenes) { m_SceneManager = scenes; }

	virtual AssetManager GetAssets() { return m_Manager; }
	char m_SceneName[15] = "MyScene";
protected:
	SceneManager* m_SceneManager;
	GLFWwindow* Window;
	AssetManager m_Manager;
};