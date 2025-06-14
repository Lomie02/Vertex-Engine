#pragma once
#include "Texture2D.h"
#include "VertexComponent.h"
#include "glm.hpp"

/*
	Vertex Engine 2D Sprite Renderer Component.
*/

class SpriteRenderer : public VertexComponent
{
public:
	SpriteRenderer(); // Default constructor.
	Texture2D Sprite; // Sprite texture
	glm::vec4 Colour; // ColourTint
	bool Transparent; // Is Transparent
	void ReCalculatePixels(); // Performs a PPU.
	void SetPixelCalculationUpdate(bool _state); // Set if the sprite renderer should automatically update the PPU when sprite changed. (Only Function)
	bool IsPixelCalculationEnabled() { return m_ReCaclulatePixelsOnSpriteChange; } // Check if the PPU re-Calculation is enabled.

	void SetSprite(Texture2D _newSprite); // Set a new sprite. This will auto update the PPU if enabled.

	void RenderEditorDisplay() override; // Render editor display
private:
	float m_EditorPreviewScale = 10.0f;
	float m_Ppu = 60.0f;
	bool m_ReCaclulatePixelsOnSpriteChange = true; // flag if auto ppu is enabled.
	ImVec2 m_WindowSize;
};

