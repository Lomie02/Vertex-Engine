#pragma once

#include "ImGuiGeneral.h"
#include "SceneManager.h"

#include "Debug.h"
#include "ImGuizmo.h"

/*
	Vertex Engine Editor 2.0

	This editor allows live editing of scenes & its objects. More control is provided from its previous basic form.
*/

class VertexEngineEditor
{
public:
	// Defaulpt constructor
	VertexEngineEditor() {};
	// De-Constructor 
	~VertexEngineEditor();

	/// <summary>
	/// Creates the editor layout & takes in params for window & scene management.
	/// </summary>
	/// <param name="_GameWindow"></param>
	/// <param name="_scenesManager"></param>
	void CreateEditorLayout(GLFWwindow* _GameWindow, SceneManager* _scenesManager);
	/// <summary>
	/// Adds textures, models, sprites & scene data.
	/// </summary>
	void PopulateDesk();
	/// <summary>
	/// Render & updates all editor displays.
	/// </summary>
	void RenderEditorDisplays();
	/// <summary>
	/// Cleans up imgui & editor related windows.
	/// </summary>
	void CleanUpGui();

	// TODO: Remove this when Runtime Pipeline is created.
	void GiveEditorMode(RunTimeMode* _mode) { m_EditorMode = _mode; }

private:

	RunTimeMode* m_EditorMode; // Current engine mode
	ImVec2 m_LastViewportSizeScene; // Scene view 
	std::vector<GameObject*> m_GameObjectsInProject; // All gameobjects in the current scene/
	int m_CurrentObjectSelected; // index of current object.

	GameObject* m_SelectedGameObject; // Ptr to selected gameobject for editing.
	Texture2D m_SelectedTextureObject;// Texture currently selected.

	// Wrapper for spacing out imgui widgets.
	void VertexSpacer(int _spaces = 1) { for (int i = 0; i < _spaces; i++) ImGui::Spacing(); }

	// Change engine colours.
	void UpdateEditorColours();

	void RenderEditorGameView(); // The Editors game view window.
	void RenderEditorSceneView(); // The Editors scene view window.

	void RenderEditorInspector(); // Inspector
	void RenderEditorInheritList(); // Hierachy
	void RenderEditorDesk(); // Desk

	void RenderGameCommandConsole(); // Render the engines console for debugging.
	void RenderGameObjectNodes(Transform* _obj, GameObject*& selection); // Render the scene hierachy.

	float m_DeskPanelItemMultiplier = 0.4f; // The mutiplier for how large desk previews should be.
	void RenderDockingTaskBar(); // Renders the docking bar.
	SceneManager* m_ApplicationCentralSceneManager; // The main scene manager ptr.

	// Editor Only
	RenderTexture* m_GameViewRenderTexture = nullptr; // Renders the game view from the main scene camera.
};

