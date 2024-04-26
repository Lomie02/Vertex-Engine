#pragma once
#include <vector>
#include "VertexScene.h"

#include "MyScene.h"
#include "Scene2.h"
/*
	The Vertex Work Space is designed to be a space where devs can created & maintain their scenes without getting involved with the application file.
	Vertex Engine will also give the option of using a vertex work space as the main object for scene creation. If turned off the engine will use any scenes created inside
	the application SceneSetup function.

*/
class VertexWorkSpace
{
public:
	/// <summary>
	/// Setup all your scenes for the engine to use.
	/// </summary>
	void SceneCreation();
	/// <summary>
	/// Allows the engine to grab your scenes used inside the workspace.
	/// </summary>
	/// <returns> Array of the VertexScenes.</returns>
	std::vector<VertexScene*> GrabWorkSpaceScenes() { return m_SceneList; }
private:

	std::vector<VertexScene*> m_SceneList;
	// Scene Objects
	Scene2* m_MyScene;
};

