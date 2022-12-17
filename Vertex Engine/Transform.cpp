#include "Transform.h"

void Transform::SetScale(float _scale)
{
	size.x += _scale;
	size.y += -_scale;
}

void Transform::Reset()
{
	position.x = 0;
	position.y = 0;

	rotation = 0;
	size.x = 100;
	size.y = 100;

	scale = 0.5f;
}
