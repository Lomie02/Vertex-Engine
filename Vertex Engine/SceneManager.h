#pragma once
#include <iostream>
#include "VertexScene.h"

#include <vector>
#include "Vertex2D.h"

class SceneManager
{
public:

	 void StartUpScenes();

	 void SetUpWindow(GLFWwindow* _window);
	 void AddScene(VertexScene* _scene);
	 void UpdateScenes(float delta);
	 void UpdateFixedScenes(float fixedDelta);
	 void SetActiveScene(int _scene) { m_ActiveScene = _scene; StartUpScenes(); }

	 void SetActiveScene(const char _scene[]);

	 void RenderCurrentScene(Vertex2D* render);
	 void PrintActiveScene() { std::cout << m_SceneList[m_ActiveScene]->m_SceneName << std::endl; };

	 void EndSceneSetUpProcess(SceneManager* _sceneManager);

	 VertexScene* GetCurrentScene() { return m_SceneList.at(m_ActiveScene); }

	 int GetActiveScene() { return m_ActiveScene; }
	 std::vector<VertexScene*> m_SceneList;

	 void AssignManager();
private:

	 int m_ActiveScene;
};