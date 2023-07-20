#include "Transform.h"

void Transform::SetSize(float x, float y)
{
	size.x = x * scale;
	size.y = y * scale;
}

void Transform::Reset()
{
	position.x = 0;
	position.y = 0;

	rotation = 0;
	size.x = 100 * scale;
	size.y = -100 * scale;

	scale = 0.5f;
}

glm::vec2 Transform::GetCenter()
{
	glm::vec2 Centre{};

	Centre.x = position.x + size.x / 2;
	Centre.y = position.y - size.y / 2;

	return Centre;
}

float Transform::GetSizeFromCentre(glm::vec2 _axis)
{
	if (_axis.x == 1)
	{
		return size.x / 2;
	}
	else if (_axis.y == 1) {

		return size.y / 2;	
	}

	return 0.0f;
}
