#include "RectTransform.h"
#include "GameObject.h"
#include "imgui.h"
#include "gtx/string_cast.hpp"
#include "gtx/matrix_decompose.hpp"
#include "glm.hpp"

#define GLM_ENABLE_EXPERIMENTAL

glm::mat4 RectTransform::GetWorldMatrix()
{
	if (m_IsDirty) ValidateDirtyTransforms();

	if (Parent) {

		m_WorldMatrix = Parent->GetWorldMatrix() * m_LocalMatrix;
		return m_WorldMatrix;
	}
	else {

		m_WorldMatrix = m_LocalMatrix;
		return m_WorldMatrix;
	}
}

glm::mat4 RectTransform::GetLocalMatrix()
{
	return m_LocalMatrix;
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
	if (Parent->partner2d == _parent) return;

	if (Parent)
	{
		auto& sib = Parent->m_Children;
		sib.erase(std::remove(sib.begin(), sib.end(), this), sib.end());
	}

	glm::mat4 previousWorld = this->GetWorldMatrix();

	if (_parent) {
		glm::mat4 parentWorld = _parent->GetComponenet<RectTransform>()->GetWorldMatrix();

		this->m_LocalMatrix = glm::inverse(parentWorld) * previousWorld;
		this->Parent = _parent->GetComponenet<RectTransform>();
	}
	else {
		m_LocalMatrix = previousWorld;
	}

	MarkDirty();

	if (_parent)
	{
		Parent->m_Children.push_back(this);
	}
}

void RectTransform::AddChild(GameObject* _child)
{
}

void RectTransform::RemoveChild(GameObject* _child)
{
}

void RectTransform::ValidateDirtyTransforms(bool _forceValidate)
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

	glm::mat4 trans = glm::translate(glm::mat4(1.0f), glm::vec3(worldPosition, 0.0f));
	glm::mat4 Rot = glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0, 0, 1));
	glm::mat4 Scal = glm::scale(glm::mat4(1.0f), glm::vec3(size * scale, 1));

	m_LocalMatrix = trans * Rot * Scal;

	if (Parent) {
		//m_Parent->ValidateDirtyTransforms();
		m_WorldMatrix = Parent->GetWorldMatrix() * m_LocalMatrix;
	}
	else {
		m_WorldMatrix = m_LocalMatrix;
	}

	for (auto* child : m_Children) {
		child->MarkDirty();
	}
}
