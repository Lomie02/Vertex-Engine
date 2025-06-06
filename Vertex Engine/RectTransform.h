#pragma once
#include "glm.hpp"
#include "VertexComponent.h"
#include "Debug.h"
class RectTransform : public VertexComponent
{
public:
	glm::vec2 anchoredPosition = glm::vec2(0,0);
	glm::vec2 size = glm::vec2(400,300);

	glm::vec2 anchoredMin = anchoredMax = glm::vec2(0.5f);
	glm::vec2 anchoredMax;

	glm::vec2 origin = glm::vec2(0.5f,0.5f);

	glm::vec2 scale = glm::vec2(1.0f, 1.0f);
	float rotation = 0;

	glm::vec2 worldPosition;

	glm::mat4 GetWorldMatrix();
	glm::mat4 GetLocalMatrix();

	void SetParent(GameObject* _parent, bool _keepWorld);
	void RenderEditorDisplay() override;

	RectTransform* Parent;
private:
	glm::mat4 m_PreviousWorld;
};

