#include "DebugComp.h"
#include <iostream>
#include "GameObject.h"

void DebugComp::Start()
{
	std::cout << "Componenet Start" << std::endl;
}

void DebugComp::Update(float delta)
{
	std::cout << "Componenet Updated" << std::endl;
}

void DebugComp::RenderEditorDisplay()
{
	if (ImGui::CollapsingHeader("Debug", ImGuiTreeNodeFlags_DefaultOpen)) {
		ImGui::Text("Test Debug Componenet");
		ImGui::Button("Button");
	}
}
