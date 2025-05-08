#pragma once
#include "glm.hpp"
class vTransform
{
public:
	vTransform();
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	void Reset();

	glm::mat4 GetGlobalTransform() { return m_GlobalTranform; }
	glm::mat4 GetLocalTransform() { return m_LocalTranform; }

	void SetGlobalTransform(glm::mat4 _matrix) { m_GlobalTranform = _matrix; }
	void SetLocalTransform(glm::mat4 _matrix) { m_LocalTranform = _matrix; }

	void ConfigureSystems();

private:
	glm::mat4 m_GlobalTranform = glm::mat4(1);
	glm::mat4 m_LocalTranform = glm::mat4(1);
};

