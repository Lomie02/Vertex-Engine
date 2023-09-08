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

/// <summary>
/// Camera class used for rendering perspective
/// </summary>
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
	glm::mat4 GetProjectionAlone() { return m_ProjectionMat; }

	float farClip = 100.0f;
	float nearClip = -0.1f;
	float zoom = 0.1;
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

