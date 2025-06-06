#include "RectTransform.h"
#include "GameObject.h"
#include "imgui.h"
#include "gtx/string_cast.hpp"
#include "gtx/matrix_decompose.hpp"
#include "glm.hpp"

#define GLM_ENABLE_EXPERIMENTAL

glm::mat4 RectTransform::GetWorldMatrix()
{
	if (Parent) {
		glm::mat4 parentWorld = Parent->GetWorldMatrix();

		if (!glm::all(glm::isinf(parentWorld[0])) || !glm::all(glm::isinf(parentWorld[1])) || !glm::all(glm::isinf(parentWorld[2])) || !glm::all(glm::isinf(parentWorld[3]))) {
			VERTEX_ERROR("Failed: Giving a fallback matrix");
			return glm::mat4(1.0f);
		}
		return parentWorld * GetLocalMatrix();
	}
	else {
		return GetLocalMatrix();
	}
}

glm::mat4 RectTransform::GetLocalMatrix()
{
	glm::vec2 parentSize;

	if (Parent) {
		parentSize = Parent->size;
	}
	else {
		parentSize = glm::vec2(1920, 1080);
	}

	glm::vec2 anchorPos = glm::mix(anchoredMin * parentSize, anchoredMax * parentSize, 0.5f);

	glm::vec2 OriginOffset = origin * size;

	worldPosition = anchorPos + anchoredPosition - OriginOffset;

	glm::mat4 model = glm::mat4(1.0f);

	model = glm::translate(model, glm::vec3(worldPosition, 0.0f));
	model = glm::translate(model, glm::vec3(OriginOffset, 0.0f));
	model = glm::rotate(model, rotation, glm::vec3(0, 0, 1));
	model = glm::translate(model, glm::vec3(-OriginOffset, 0));
	model = glm::scale(model, glm::vec3(size * scale, 1));

	return model;
}

void RectTransform::RenderEditorDisplay()
{
	if (ImGui::CollapsingHeader("Rect Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
		// vars
		ImGui::Text("Final Position");
		ImGui::InputFloat("##finalX", &this->worldPosition.x);
		ImGui::InputFloat("##finalY", &this->worldPosition.y);

		ImGui::Spacing();
		ImGui::Text("Anchor Position");
		ImGui::InputFloat("##ancX", &this->anchoredPosition.x);
		ImGui::InputFloat("##ancY", &this->anchoredPosition.y);

		ImGui::Text("Anchor Min");
		ImGui::InputFloat("##ancMinX", &this->anchoredMin.x);
		ImGui::InputFloat("##ancMinY", &this->anchoredMin.y);

		ImGui::Text("Anchor Max");
		ImGui::InputFloat("##ancMaxX", &this->anchoredMax.x);
		ImGui::InputFloat("##ancMaxY", &this->anchoredMax.y);
		// Far Clipping
		ImGui::Spacing();

		ImGui::Text("Origin");
		ImGui::InputFloat("##originRectX", &this->origin.x);
		ImGui::InputFloat("##originRectY", &this->origin.y);

		ImGui::Text("Size");
		ImGui::InputFloat("##sizeX", &this->size.x);
		ImGui::InputFloat("##sizeY", &this->size.y);

		ImGui::Text("Scale");
		ImGui::InputFloat("##scaleX", &this->scale.x);
		ImGui::InputFloat("##scaleY", &this->scale.y);

		ImGui::Text("Rotation");
		ImGui::InputFloat("##rotate", &this->rotation);

		ImGui::Spacing();
	}
}

void RectTransform::SetParent(GameObject* _parent, bool _keepWorld)
{
	glm::mat4 oldWorld = GetLocalMatrix();

	Parent = _parent->GetComponenet<RectTransform>();

	if (_keepWorld && _parent) {

		glm::mat4 newWorld = Parent ? Parent->GetWorldMatrix() : glm::mat4(1.0f);
		glm::mat4 newLocal = glm::inverse(newWorld) * oldWorld;

		glm::vec3 skey;
		glm::vec3 trans;
		glm::quat rot;
		glm::vec3 Scale; 
		glm::vec4 per;

		if (glm::decompose(newLocal, Scale, rot, trans, skey, per)) {
			rotation = glm::eulerAngles(rot).z;
			scale = glm::vec2(Scale);

			glm::vec2 parentSize;

			if (Parent) {
				parentSize = Parent->size;
			}
			else {
				parentSize = glm::vec2(1920, 1080);
			}

			this->anchoredPosition = _parent->GetComponenet<RectTransform>()->anchoredPosition + parentSize * ((anchoredMin + anchoredMax) * 0.5f);


			VERTEX_ERROR("Comp Success");
		}
		else {
			VERTEX_ERROR("Failed to parent");
		}

	}
	else if (_parent == nullptr) {

		glm::vec3 skey;
		glm::vec3 trans;
		glm::quat rot;
		glm::vec3 Scale;
		glm::vec4 per;

		if (glm::decompose(oldWorld, Scale, rot, trans, skey, per)) {
			rotation = glm::eulerAngles(rot).z;
			scale = glm::vec2(Scale.x, Scale.y);
			anchoredPosition = glm::vec2(trans.x, trans.y);

			worldPosition = anchoredPosition;
		}
		else {
			VERTEX_ERROR("Failed to parent");
		}
	}
}
