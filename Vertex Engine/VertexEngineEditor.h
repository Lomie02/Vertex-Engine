#pragma once
#include "ImGuiGeneral.h"
#include "SceneManager.h"
class VertexEngineEditor
{
public:
	void CreateEditorLayout(GLFWwindow* _GameWindow);
	void RenderEditorDisplays();

	
private:

	void RenderEditorGameView(); // The Editors game view window.
	void RenderEditorSceneView(); // The Editors scene view window.

	void RenderEditorInspector(); // Inspector
	void RenderEditorInheritList(); // Hierachy
	void RenderEditorDesk(); // Desk


	SceneManager* m_ApplicationCentralSceneManager;
};

