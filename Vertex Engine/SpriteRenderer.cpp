#include "SpriteRenderer.h"
#include "GameObject.h"
#include "ResourceManager.h"
SpriteRenderer::SpriteRenderer()
{
	Sprite = ResourceManager::GetTexture("UI_Button");

	// PPU pass
	if (partner2d) {
		partner2d->transform->size.x = Sprite.Width / 60;
		partner2d->transform->size.y = Sprite.Height / 60;
	}
}

void SpriteRenderer::RenderEditorDisplay()
{
	ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0, 0.5f, 0, 1));
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 0.5f, 0, 1));
	if (ImGui::CollapsingHeader("Sprite Renderer", ImGuiTreeNodeFlags_DefaultOpen)) {

		// ============================= Transform Positions

		// X Axis
		ImVec2 screenSize = ImGui::GetContentRegionAvail();

		ImGui::Image(Sprite.ID, ImVec2(64, 64)); ImGui::SameLine;
		ImGui::Dummy(ImVec2(screenSize.x, 1));

		ImGui::InputFloat("R",&this->Colour.x);
		ImGui::InputFloat("G",&this->Colour.y);
		ImGui::InputFloat("B",&this->Colour.z);
		ImGui::InputFloat("A",&this->Colour.w);

	}
	ImGui::PopStyleColor(2);
}
