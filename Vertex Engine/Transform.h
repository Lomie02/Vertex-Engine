#pragma once
#include "glm.hpp"

class Transform {
public:
	glm::vec2 position;
	glm::vec2 PreviousPosition;
	glm::vec2 localPosition;
	float rotation;
	glm::vec2 size;
	void SetScale(float _scale);
	void Reset();
private:

	float scale;
};