#include "Camera.h"
#include "GameSettings.h"
#include "GameObject.h"
#include "vGameObject.h"

void Camera::Start()
{
}

void Camera::Update(float delta)
{
}

glm::mat4 Camera::GetProjection()
{
	if (renderTexture == nullptr) {
		m_AspectRatio = (float)PROJECT_ASPECT_WIDTH / (float)PROJECT_ASPECT_HEIGHT;
	}
	else {
		m_AspectRatio = (float)renderTexture->GetWidth() / (float)renderTexture->GetHeight();
	}

	
	if (m_LensMode == LensMode::Ortho) {

		if (zoom < 0.0f) {
			zoom = 0.001;
		}
		if (zoom > 999) {
			zoom = 999;
		}

		float halfWidth = (m_AspectRatio * zoom) * 0.5f;
		float halfHeight = zoom * 0.5f;

		m_ProjectionMat = glm::ortho(-zoom * m_AspectRatio, zoom * m_AspectRatio, -zoom, zoom, nearClip, farClip);

		glm::mat4 mTransform = glm::translate(glm::mat4(1.0f), glm::vec3(partner2d->transform->position)) * glm::toMat4(partner2d->transform->rotation);

		m_ViewMat = glm::inverse(mTransform);

		m_ViewProjection = m_ProjectionMat * m_ViewMat;

		return m_ViewProjection;
	}
	else
	{
		m_CameraPos = glm::vec3(partner2d->transform->position);
		glm::vec3 cameraDirection = glm::normalize(m_CameraPos - cameraTarget);

		glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
		glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

		m_ProjectionMat = glm::perspective(glm::radians(m_FieldofView), m_AspectRatio, nearClip, farClip);

		m_ViewMat = glm::translate(m_ViewMat, partner2d->transform->position);

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

void Camera::SetDisplay(int _index)
{
	m_DisplayScreen = _index;

	// Keep the display within range of screen 0-9
	if (m_DisplayScreen < 0)
		m_DisplayScreen = 0;
	if(m_DisplayScreen > 9)
		m_DisplayScreen = 9;

}

void Camera::RenderEditorDisplay()
{
	if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen)) {
		// vars
		const char* lensModes[] = { "Ortho", "Projection" };
		static int projectionMode = (int)this->GetLens();

		//Projection Mode
		ImGui::Text("Lens Mode"); ImGui::SameLine();
		if (ImGui::Combo("##lensMode", &projectionMode, lensModes, IM_ARRAYSIZE(lensModes))) {
			if (projectionMode == 0) {
				this->SetLens(LensMode::Ortho);
			}
			else {
				this->SetLens(LensMode::Perspective);
			}
		}
		// Near Clipping
		ImGui::Text("Near Clip"); ImGui::SameLine();
		ImGui::InputFloat("##nearClip", &this->nearClip);
		// Far Clipping
		ImGui::Text("Far Clip"); ImGui::SameLine();
		ImGui::InputFloat("##farClip", &this->farClip);
		// Zoom
		if (this->GetLens() == LensMode::Ortho) {
			ImGui::Text("Zoom Clip"); ImGui::SameLine();
			ImGui::InputFloat("##zoomCamera", &this->zoom);

		}
		// DisPlays 
		ImGui::Text("Display"); ImGui::SameLine();
		static int display = this->GetDisplay();
		if (ImGui::InputInt("##camDisplay", &display)) {
			this->SetDisplay(display);
		}
	}
}
