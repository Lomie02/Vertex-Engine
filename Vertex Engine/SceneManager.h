#pragma once
#include <iostream>
#include "VertexScene.h"

#include <vector>
#include "Vertex2D.h"

class SceneManager
{
public:

	/// <summary>
	/// Current active scenes start function gets called.
	/// </summary>
	void StartUpScenes();

	void AddScene(VertexScene* _scene);
	void UpdateScenes(float delta);
	void UpdateFixedScenes(float fixedDelta);
	/// <summary>
	/// Change scene via its position in the scene list.
	/// </summary>
	/// <param name="_scene"></param>
	void SetActiveScene(int _scene) { m_ActiveScene = _scene; if (m_IsAllowedToStartScenes) StartUpScenes(); }
	/// <summary>
	/// Change scenes via name.
	/// </summary>
	/// <param name="_scene"> Scene Name </param>
	void SetActiveScene(const char _scene[]);
	/// <summary>
	/// Prints the current active scenes name in the terminal.
	/// </summary>
	void PrintActiveScene() { std::cout << m_SceneList[m_ActiveScene]->m_SceneName << std::endl; };
	/// <summary>
	/// Get the ptr to the current active scene.
	/// </summary>
	/// <returns> VertexScene Object </returns>
	VertexScene* GetCurrentScene() { return m_SceneList.at(m_ActiveScene); }
	/// <summary>
	/// Get the current active scene int.
	/// </summary>
	/// <returns></returns>
	int GetActiveScene() { return m_ActiveScene; }
	/// <summary>
	/// The raw scene list.
	/// </summary>
	std::vector<VertexScene*> m_SceneList;
	/// <summary>
	/// When Changing scenes the previous scene will be deleted.
	/// </summary>
	/// <param name="_state"></param>
	void DestroyScenesOnLoad(bool _state) { m_DestroyScenes = _state; }
	/// <summary>
	/// Set the Engines runtime mode.
	/// </summary>
	/// <param name="_mode">Takes an EditorMode keyword</param>
	void EngineState(EditorMode _mode);
	/// <summary>
	/// Deletes all vertex scenes & clears the scene manager.
	/// </summary>
	/// 
	void CleanUpSceneManagerAssets();

private:
	bool m_IsAllowedToStartScenes = true;
	bool m_DestroyScenes = false;
	int m_ActiveScene;
};