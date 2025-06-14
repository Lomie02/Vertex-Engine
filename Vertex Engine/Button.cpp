#include "Button.h"

Button::Button()
{
	Awake();
}

Button::~Button()
{
	if (m_SpriteRendererComp) {
		delete m_SpriteRendererComp;
		m_SpriteRendererComp = nullptr;
	}
}

void Button::Awake()
{
	
}

void Button::RenderEditorDisplay()
{
	ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0, 0.5f, 0, 1));
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 0.5f, 0, 1));

	if (ImGui::CollapsingHeader("Button", ImGuiTreeNodeFlags_DefaultOpen)) {

		ImGui::Text("Normal Colour "); ImGui::SameLine();
		ImGui::PushID(0);
		ImGuiVec4(m_NormalColour);
		ImGui::PopID();

		ImGui::Text("Selected Colour: "); ImGui::SameLine();
		ImGui::PushID(1);
		ImGuiVec4(m_SelectedColour);
		ImGui::PopID();
	}
	ImGui::PopStyleColor(2);
}

void Button::Update(float deltaTime)
{

}

void Button::OnHoverEnter()
{
}

void Button::OnHoverExit()
{
}

void Button::OnClicked()
{
	onClick->Invoke();
}

void Button::OnSelected()
{
}

void Button::ImGuiVec4(glm::vec4 _colourVec)
{
	ImGui::SetNextItemWidth(25.0f); 
	ImGui::DragFloat("R:", &_colourVec.r, 1.0f, 0.0f, 255.0f);
	ImGui::SetNextItemWidth(25.0f); ImGui::SameLine();
	ImGui::DragFloat("G:", &_colourVec.g, 1.0f, 0.0f, 255.0f);
	ImGui::SetNextItemWidth(25.0f); ImGui::SameLine();
	ImGui::DragFloat("B:", &_colourVec.b, 1.0f, 0.0f, 255.0f);
	ImGui::SetNextItemWidth(25.0f); ImGui::SameLine();
	ImGui::DragFloat("A:", &_colourVec.a, 1.0f, 0.0f, 255.0f);

}
