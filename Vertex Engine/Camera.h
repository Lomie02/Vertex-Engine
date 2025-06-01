#pragma once
#include "Graphics.h"
#include "ext.hpp"

#include "GameSettings.h"
#include <string>
#include "ext.hpp"
#include "RenderTexture.h"
#include "VertexComponent.h"
enum class LensMode {
	Ortho = 0,
	Perspective,
};

//TODO: Camera class needs to be converted to a component instead of a GameObject.

class Camera : public VertexComponent
{
public:
	Camera()
	{
		Start();
		m_ViewProjection = m_ProjectionMat * m_ViewMat;
	}

	void Start() override;
	void Update(float delta) override;

	void SetLens(LensMode _mode) { m_LensMode = _mode; }
	glm::mat4 GetProjection();

	void SetFieldOfView(float _fov);
	float GetFieldOfView() { return m_FieldofView; };
	glm::mat4 GetProjectionAlone() { return m_ProjectionMat; }

	float farClip = 10.0f;
	float nearClip = -0.1f;
	float zoom = 0.1;

	RenderTexture* renderTexture = nullptr;

	LensMode GetLens() { return m_LensMode; }
	void SetAspectRatio(float _aspect) { m_AspectRatio = _aspect; }
	glm::mat4 GetViewMatrix() { return m_ViewMat; }

	void SetDisplay(int _index);
	int GetDisplay() { return m_DisplayScreen; }

	void RenderEditorDisplay() override;
private:

	int m_DisplayScreen = 0;
	float m_AspectRatio = (float)PROJECT_ASPECT_WIDTH / (float)PROJECT_ASPECT_HEIGHT;

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
	LensMode m_LensMode = LensMode::Ortho;

};

