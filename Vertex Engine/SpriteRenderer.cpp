#include "SpriteRenderer.h"
#include "GameObject.h"
#include "ResourceManager.h"

// Defines the scale for previews in the editor. 
#define EDITOR_SPRITE_WIDTH (Sprite.Width / m_Ppu) * m_EditorPreviewScale , (Sprite.Height / m_Ppu) * m_EditorPreviewScale

SpriteRenderer::SpriteRenderer()
{
	// Get the default sprite testure.
	Sprite = ResourceManager::GetTexture("UI_Button");

	// Set full white with no Alpha.
	Colour = glm::vec4(1, 1, 1, 1);

	// PPU pass
	ReCalculatePixels();
}

void SpriteRenderer::ReCalculatePixels()
{
	if (partner2d) {
		partner2d->transform->scale.x = Sprite.Width / 60.0f;
		partner2d->transform->scale.y = Sprite.Height / 60.0f;
	}
}

void SpriteRenderer::SetPixelCalculationUpdate(bool _state)
{
	m_ReCaclulatePixelsOnSpriteChange = _state;
}

void SpriteRenderer::SetSprite(Texture2D _newSprite)
{
	Sprite = _newSprite;
	ReCalculatePixels();
}

void SpriteRenderer::RenderEditorDisplay()
{
	ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0, 0.5f, 0, 1));
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 0.5f, 0, 1));

	if (ImGui::CollapsingHeader("Sprite Renderer", ImGuiTreeNodeFlags_DefaultOpen)) {

		// ============================= Transform Positions
		static bool isTransparent;
		// X Axis
		m_WindowSize = ImGui::GetContentRegionAvail();

		ImGui::Image(Sprite.ID, ImVec2(EDITOR_SPRITE_WIDTH), ImVec2(0,1), ImVec2(1,0)); ImGui::SameLine;

		ImGui::Spacing();

		ImGui::Dummy(ImVec2(m_WindowSize.x, 5));
		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* gameObjectPayload = ImGui::AcceptDragDropPayload("TEXTURE")) {
				const char* texture = (const char*)gameObjectPayload->Data;

				Sprite = ResourceManager::GetTexture(texture);
				ReCalculatePixels();

			}
			ImGui::EndDragDropTarget();
		}

		ImGui::Spacing();

		ImGui::InputFloat("R", &this->Colour.x);
		ImGui::InputFloat("G", &this->Colour.y);
		ImGui::InputFloat("B", &this->Colour.z);
		ImGui::InputFloat("A", &this->Colour.w);



	}
	ImGui::PopStyleColor(2);
}
