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
