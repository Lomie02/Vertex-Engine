#include "VertexEngineEditor.h"

void VertexEngineEditor::CreateEditorLayout(GLFWwindow* _GameWindow)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui_ImplGlfw_InitForOpenGL(_GameWindow, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	ImGui::StyleColorsDark();
}

void VertexEngineEditor::RenderEditorDisplays()
{
}

void VertexEngineEditor::RenderEditorGameView()
{
}

void VertexEngineEditor::RenderEditorSceneView()
{
}

void VertexEngineEditor::RenderEditorInspector()
{
}

void VertexEngineEditor::RenderEditorInheritList()
{
}

void VertexEngineEditor::RenderEditorDesk()
{
}
