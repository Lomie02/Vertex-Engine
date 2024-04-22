#include "SceneManager.h"

void SceneManager::StartUpScenes()
{
	if (m_SceneList.size() >= 0 && m_ActiveScene <= m_SceneList.size())
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
				if (m_IsAllowedToStartScenes)
				{
					StartUpScenes();
				}
				return;
			}
		}
	}
}

void SceneManager::EndSceneSetUpProcess(SceneManager* _sceneManager)
{

}

void SceneManager::EngineState(EditorMode _mode)
{
	if (_mode == PLAY || _mode == EDITOR_PLAY)
	{
		m_IsAllowedToStartScenes = true;
	}
	else {
		m_IsAllowedToStartScenes = false;
	}
}

void SceneManager::DestroyScenes()
{
	for (int i = 0; i < m_SceneList.size(); i++)
	{
	}
}
