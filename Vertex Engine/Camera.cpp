#include "Camera.h"

glm::mat4 Camera::GetProjection()
{
	if (zoom < 0.1f) {
		zoom = 0.1f;
	}
	if (zoom > 999) {
		zoom = 999;
	}

	m_ProjectionMat = glm::ortho(-zoom, zoom, -zoom, zoom, near, far);

	glm::mat4 mTransform = glm::translate(glm::mat4(1.0f), glm::vec3(transform.position, 0.0f)) * glm::rotate(glm::mat4(1.0f), transform.rotation, glm::vec3(0, 0, 1));

	m_ViewMat = glm::inverse(mTransform);

	m_ViewProjection = m_ProjectionMat * m_ViewMat;

	return m_ViewProjection;
}