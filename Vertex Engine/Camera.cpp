#include "Camera.h"
#include "GameSettings.h"

glm::mat4 Camera::GetProjection()
{
	float aspect = (float)PROJECT_ASPECT_WIDTH / (float)PROJECT_ASPECT_HEIGHT;
	if (m_LensMode == Ortho) {

		if (zoom < 0.0f) {
			zoom = 0.001;
		}
		if (zoom > 999) {
			zoom = 999;
		}

		m_ProjectionMat = glm::ortho(-aspect / zoom, aspect / zoom, -1.0f / zoom, 1.0f / zoom, near, far);


		glm::mat4 mTransform = glm::translate(glm::mat4(1.0f), glm::vec3(transform.position, 10)) * glm::rotate(glm::mat4(1.0f), transform.rotation, glm::vec3(0, 0, 1));

		m_ViewMat = glm::inverse(mTransform);

		m_ViewProjection = m_ProjectionMat * m_ViewMat;

		return m_ViewProjection;
	}
	else
	{
		m_CameraPos = glm::vec3(transform.position, zoom);
		glm::vec3 cameraDirection = glm::normalize(m_CameraPos - cameraTarget);

		glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
		glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

		m_ProjectionMat = glm::perspective(m_FieldofView, aspect, near, far);

		m_ViewMat = glm::lookAt(m_CameraPos, m_CameraPos + cameraFront, cameraUp);

		m_ViewProjection = m_ProjectionMat * m_ViewMat;

		return m_ViewProjection;
	}
}

void Camera::SetFieldOfView(float _fov)
{
	if (_fov < 1) {
		_fov = 1;
	}
	else if (_fov > 200) {
		_fov = 200;
	}

	m_FieldofView = _fov;
}
