#pragma once

#include "ImGuiGeneral.h"
#include "SceneManager.h"

#include "Debug.h"
#include "ImGuizmo.h"

class VertexEngineEditor
{
public:
	void CreateEditorLayout(GLFWwindow* _GameWindow, SceneManager* _scenesManager);
	void PopulateDesk();
	void RenderEditorDisplays();

	void CleanUpGui();

	void GiveEditorMode(EditorMode& _mode) { m_EditorMode = _mode; }

private:


	EditorMode m_EditorMode;
	ImVec2 m_LastViewportSizeScene;
	std::vector<GameObject*> m_GameObjectsInProject;
	int m_CurrentObjectSelected;

	GameObject* m_SelectedGameObject;

	static bool DisplaySceneDrawer;
	static const char* m_SceneList[MAX_SCENES];

	static int selectedCamera;
	static int selectedSprite;

	static int selectedUserInterface;
	static int selectedTextInterface;

	static int currentScene;
	static float ScaleAmount;

	//=================================================

	void VertexSpacer(int _spaces = 1) { for (int i = 0; i < _spaces; i++) ImGui::Spacing(); }

	void UpdateEditorColours();

	void RenderEditorGameView(); // The Editors game view window.
	void RenderEditorSceneView(); // The Editors scene view window.

	void RenderEditorInspector(); // Inspector
	void RenderEditorInheritList(); // Hierachy
	void RenderEditorDesk(); // Desk

	void RenderGameCommandConsole();
	void RenderGameObjectNodes(GameObject* _obj, GameObject*& selection);

	bool IsChildOf(GameObject* _parent, GameObject* _child);
	float m_DeskPanelItemMultiplier = 0.4f;
	void RenderDockingTaskBar();
	SceneManager* m_ApplicationCentralSceneManager;

	// Editor Only
	RenderTexture* m_GameViewRenderTexture = nullptr;
};

