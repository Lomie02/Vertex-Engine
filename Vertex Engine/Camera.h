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
		m_ProjectionMat = glm::ortho(-zoom, zoom, -zoom, zoom, near, far);

		m_ViewProjection = m_ProjectionMat * m_ViewMat;
	}

	Camera(const char* _Name) : GameObject(_Name)
	{
		m_ProjectionMat = glm::ortho(-zoom, zoom, -zoom, zoom, near, far);
		m_ViewProjection = m_ProjectionMat * m_ViewMat;
	}

	glm::mat4 GetProjection();

	float far = 1.0f;
	float near = -1.0f;
	float zoom = 10.0f;
private:

	glm::mat4 m_ProjectionMat;
	glm::mat4 m_ViewMat;
	glm::mat4 m_ViewProjection;
	LensMode m_LensMode = Ortho;

};

