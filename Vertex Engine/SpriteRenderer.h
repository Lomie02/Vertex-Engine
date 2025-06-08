#pragma once
#include "Texture2D.h"
#include "VertexComponent.h"
#include "glm.hpp"

class SpriteRenderer : public VertexComponent
{
public:
	SpriteRenderer();
	Texture2D Sprite;
	glm::vec4 Colour;

	void RenderEditorDisplay() override;
private:
	
};

