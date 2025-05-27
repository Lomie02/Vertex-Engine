#include "Transform.h"
#include <ext.hpp>
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

	Centre.x = position.x - size.x / 2;
	Centre.y = position.y + size.y / 2;

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

glm::mat4 Transform::GetLocalModelMat() const
{
	glm::vec2 ImageOffset;
	glm::vec2 pos = position;

	ImageOffset.x = size.x / 2;
	ImageOffset.y = size.y / 2;

	pos.x -= ImageOffset.x;
	pos.y += ImageOffset.y;

	glm::vec2 GeneralPivot;
	Transform ParentsTransform;

	//========================================================


	//========================================================
	GeneralPivot = pos + pivot;

	glm::mat4 model = glm::mat4(1.0f);

	model = glm::translate(model, glm::vec3(GeneralPivot, (float)m_RenderLayer)); // position

	model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * -size.y, 0.0f)); // Position
	model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotation
	model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * -size.y, 0.0f));

	model = glm::scale(model, glm::vec3(size.x * scale, -size.y * scale, 1.0f));

	return model;
}

glm::mat4 Transform::GetWorldModelMat(const GameObject* _obj) const
{
	return glm::mat4();
}
