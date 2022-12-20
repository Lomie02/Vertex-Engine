#include "SceneManager.h"

void SceneManager::StartUpScenes()
{
	if (m_SceneList.size() >= 1)
	{
		m_SceneList[m_ActiveScene]->Start();
	}
}

void SceneManager::SetUpWindow(GLFWwindow* _window)
{
	for (int i = 0; i < m_SceneList.size(); i++)
	{
	}
}

void SceneManager::AddScene(VertexScene* _scene)
{
	m_SceneList.push_back(_scene);
}

void SceneManager::UpdateScenes(float delta)
{
	if (m_SceneList.size() >= 1) {

		m_SceneList[m_ActiveScene]->Update(delta);
		m_SceneList[m_ActiveScene]->LateUpdate(delta);
	}
}

void SceneManager::UpdateFixedScenes(float fixedDelta)
{
	m_SceneList[m_ActiveScene]->FixedUpdate(fixedDelta);
}

void SceneManager::SetActiveScene(const char _scene[])
{
	if (m_SceneList.size() >= 1) {

		for (int i = 0; i < m_SceneList.size(); i++)
		{
			if (m_SceneList[i]->m_SceneName == _scene)
			{
				m_ActiveScene = i;
				StartUpScenes();
				return;
			}
		}
	}
}

void SceneManager::RenderCurrentScene(Vertex2D* render)
{
	if (m_SceneList.size() >= 1)
	{
		m_SceneList[m_ActiveScene]->Rendering(render);
	}
}

void SceneManager::EndSceneSetUpProcess(SceneManager* _sceneManager)
{

}
