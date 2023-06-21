#pragma once
#include "glm.hpp"

class Transform {
public:
	glm::vec2 position;
	glm::vec2 PreviousPosition;
	glm::vec2 localPosition;
	float rotation;
	float localRotation;
	glm::vec2 size;

	void SetSize(float x, float y);
	glm::vec2 GetSize() {return size;}

	float scale = 1.0f;
	float localScale = 1.0f;
	void Reset();

	glm::vec2 GetCentre();
	float GetSizeFromCentre(glm::vec2 _axis);
private:

};