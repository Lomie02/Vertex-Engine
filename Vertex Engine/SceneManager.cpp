#include "SceneManager.h"

/// <summary>
/// Calls the active scenes start functions.
/// </summary>
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

/// <summary>
/// Adds a scene to the scene manager so it can be recognized by the engine and its systems.
/// </summary>
/// <param name="A Vertex Scene"></param>
void SceneManager::AddScene(VertexScene* _scene)
{
	m_SceneList.push_back(_scene);
}


/// <summary>
/// Updates all scenes in manager & passes through the delta time relative to the framerate.
/// </summary>
/// <param name="Delta Time"></param>
void SceneManager::UpdateScenes(float delta)
{
	if (m_SceneList.size() >= 1) {

		m_SceneList[m_ActiveScene]->Update(delta);
		m_SceneList[m_ActiveScene]->LateUpdate(delta);
	}
}

/// <summary>
/// Fixed Delta is passed through all the scenes fixed update loops.
/// </summary>
/// <param name="Fixed Delta Time"></param>
void SceneManager::UpdateFixedScenes(float fixedDelta)
{
	m_SceneList[m_ActiveScene]->FixedUpdate(fixedDelta);
}

/// <summary>
/// Sets the current scene to the scene with the given name.
/// </summary>
/// <param name="Scene Name"></param>
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
				break;
			}
		}
	}
}

void SceneManager::EndSceneSetUpProcess(SceneManager* _sceneManager)
{
	//TODO: Either remove or do something with it, I cant remember why I added this here.
}

/// <summary>
/// Sets the Engines current state such as if its in normal release mode or in editor testing modes.
/// </summary>
/// <param name="Editor Mode"></param>
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

void SceneManager::DestroyScenes() //TODO: Take care of this function or just remove it.
{
	for (int i = 0; i < m_SceneList.size(); i++)
	{
	}
}
