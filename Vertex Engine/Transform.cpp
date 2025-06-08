#include "Transform.h"
#include "GameObject.h"
#include "RectTransform.h"
#include <ext.hpp>

void Transform::SetSize(float x, float y)
{
	scale.x = x;
	scale.y = y;
}

void Transform::Reset()
{
	position.x = 0;
	position.y = 0;
	position.z = 0;

	rotation = glm::quat(glm::vec3(0,0,0));
}

glm::vec2 Transform::GetCenter()
{
	glm::vec2 Centre{};

	Centre.x = position.x - scale.x / 2;
	Centre.y = position.y + scale.y / 2;

	return Centre;
}

float Transform::GetSizeFromCentre(glm::vec2 _axis)
{
	if (_axis.x == 1)
	{
		return scale.x / 2;
	}
	else if (_axis.y == 1) {

		return scale.y / 2;
	}

	return 0.0f;
}

glm::mat4 Transform::GetLocalModelMat()
{
	return m_LocalModel;
}

void Transform::RenderEditorDisplay()
{
	if (partner2d->GetComponenet<RectTransform>()) return;

	ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0, 0.5f, 0, 1));
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 0.5f, 0, 1));
	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {


		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1, 0.0f, 0, 1));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.5f, 0.0f, 0, 1));

		// ============================= Transform Positions
		// X Axis
		ImGui::Button("X");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(50.0f);
		ImGui::InputFloat("##Position X Axis", &this->position.x, -0.5f);
		ImGui::PopStyleColor(2);

		// Y Axis
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 1, 0, 1));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.5f, 0, 1));

		ImGui::SameLine();
		ImGui::Button("Y");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(50.0f);
		ImGui::InputFloat("##Position Y Axis", &this->position.y);
		ImGui::PopStyleColor(2);

		ImGui::SameLine();
		ImGui::Button("Z");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(50.0f);
		ImGui::InputFloat("##Position Z Axis", &this->position.z);

		ImGui::Text("Scale");
		ImGui::SameLine();
		ImGui::InputFloat("##ScaleX", &this->scale.x);
		ImGui::InputFloat("##ScaleY", &this->scale.y);
		ImGui::InputFloat("##ScaleZ", &this->scale.z);

		ImGui::Text("Rotation");

		ImGui::Button("X");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(50.0f);
		ImGui::InputFloat("##rotX", &this->m_EulerAngle.x);

		ImGui::Button("Y");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(50.0f);
		ImGui::InputFloat("##rotY", &this->m_EulerAngle.y);

		ImGui::Button("Z");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(50.0f);
		ImGui::InputFloat("##rotZ", &this->m_EulerAngle.z);

		rotation = glm::quat(glm::radians(m_EulerAngle));

	}
	ImGui::PopStyleColor(2);
}

void Transform::SetParent(Transform* _parent)
{
	if (m_Parent == _parent) return;

	if (m_Parent)
	{
		auto& sib = m_Parent->m_Children;
		sib.erase(std::remove(sib.begin(), sib.end(), this), sib.end());
	}

	glm::mat4 previousWorld = this->GetWorldModelMat();

	if (_parent) {
		glm::mat4 parentWorld = _parent->GetWorldModelMat();

		this->m_LocalModel = glm::inverse(parentWorld) * previousWorld;
		this->m_Parent = _parent;
	}
	else {
		m_LocalModel = previousWorld;
	}

	m_IsDirty = true;

	if (_parent)
	{
		m_Parent->m_Children.push_back(this);
	}
}

void Transform::RemoveParent()
{
	if (m_Parent)
	{
		glm::mat4 previousWorld = this->GetWorldModelMat();
		auto& sib = m_Parent->m_Children;
		sib.erase(std::remove(sib.begin(), sib.end(), this), sib.end());

		m_Parent = nullptr;
		m_IsDirty = true;

		m_LocalModel = previousWorld;

	}
}

void Transform::SetChild(Transform* _child)
{
	if (_child) {
		_child->SetParent(this);
		m_IsDirty = true;
		ValidateDirtyTransforms();
	}
}

void Transform::RemoveChild(Transform* _child)
{
	if (_child && _child->m_Parent == this) {
		_child->SetParent(nullptr);
		m_IsDirty = true;
		ValidateDirtyTransforms();
	}
}

bool Transform::IsChildOf(const Transform* _child) const
{
	const Transform* current = m_Parent;
	while (current) {

		if (current == _child) return true;
		current = current->m_Parent;
	}
	return false;
}

void Transform::LateUpdate(float delta)
{
}

bool Transform::HasChanged()
{
	if (m_LastPosition.x != position.x || m_LastPosition.y != position.y || m_LastPosition.z != position.z ||
		m_LastRotation.x != rotation.x || m_LastRotation.y != rotation.y || m_LastRotation.z != rotation.z ||
		m_LastSize.x != scale.x || m_LastSize.y != scale.y || m_LastSize.z != scale.z)
	{
		m_LastPosition = position;
		m_LastRotation = rotation;
		m_LastSize = scale;

		m_IsDirty = true;
		return true;
	}


	return false;
}

void Transform::ValidateDirtyTransforms(bool _forceValidate)
{
	if (!_forceValidate) {
		if (!m_IsDirty) return;
	}

	//========================================================

	glm::mat4 trans = glm::translate(glm::mat4(1.0f), position); // position
	glm::mat4 rot = glm::toMat4(rotation); // Rotation
	glm::mat4 Scale = glm::scale(glm::mat4(1.0f), scale); // Scale

	m_LocalModel = trans * rot * Scale;

	if (m_Parent) {
		//m_Parent->ValidateDirtyTransforms();
		m_WorldMatrix = m_Parent->GetWorldModelMat() * m_LocalModel;
	}
	else {
		m_WorldMatrix = m_LocalModel;
	}

	m_IsDirty = false;
}
