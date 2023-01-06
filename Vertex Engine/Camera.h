#pragma once
#include "Graphics.h"
#include "GameObject.h"
#include "ext.hpp"

#include "GameSettings.h"
#include <string>
#include "ext.hpp"

enum LensMode {
	Ortho = 0,
	Perspective,
};

class Camera : public GameObject
{
public:
	Camera() : GameObject("Camera")
	{

		m_ViewProjection = m_ProjectionMat * m_ViewMat;
	}

	Camera(const char* _Name) : GameObject(_Name)
	{
		m_ViewProjection = m_ProjectionMat * m_ViewMat;
	}

	void SetLens(LensMode _mode) { m_LensMode = _mode; }
	glm::mat4 GetProjection();

	void SetFieldOfView(float _fov);
	float GetFieldOfView() { return m_FieldofView; };

	float far = 50.0f;
	float near = -1.0f;
	float zoom = 10.0f;
private:

	glm::vec3 m_CameraPos = glm::vec3(0, 0, 0);
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	float m_FieldofView = 120.0f;

	glm::mat4 m_PerMat;
	glm::mat4 m_ProjectionMat;
	glm::mat4 m_ViewMat;
	glm::mat4 m_ViewProjection;
	LensMode m_LensMode = Ortho;

};

