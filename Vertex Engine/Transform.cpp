#include "Transform.h"
#include "GameObject.h"
#include "RectTransform.h"
#include <ext.hpp>

void Transform::SetSize(float x, float y)
{
	size.x = x * scale;
	size.y = y * scale;
}

void Transform::Reset()
{
	position.x = 0;
	position.y = 0;

	rotation = 0;
	size.x = 100 * scale;
	size.y = -100 * scale;

	scale = 0.5f;
}

glm::vec2 Transform::GetCenter()
{
	glm::vec2 Centre{};

	Centre.x = position.x - size.x / 2;
	Centre.y = position.y + size.y / 2;

	return Centre;
}

float Transform::GetSizeFromCentre(glm::vec2 _axis)
{
	if (_axis.x == 1)
	{
		return size.x / 2;
	}
	else if (_axis.y == 1) {

		return size.y / 2;
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

		ImGui::Text("Scale");
		ImGui::SameLine();
		ImGui::InputFloat("##Scale", &this->scale);

		ImGui::Text("Size");
		ImGui::InputFloat("##sizeX", &this->size.x);
		ImGui::InputFloat("##sizeY", &this->size.y);

		ImGui::Text("Rotation");
		ImGui::InputFloat("##rot", &this->rotation);
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
	if (m_LastPosition.x != position.x || m_LastPosition.y != position.y ||
		m_LastRotation != rotation || m_LastScale != scale ||
		m_LastSize.x != size.x || m_LastSize.y != size.y)
	{
		m_LastPosition = position;
		m_LastRotation = rotation;
		m_LastScale = scale;
		m_LastSize = size;

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

	glm::vec2 ImageOffset;
	glm::vec2 pos = position;

	ImageOffset.x = size.x / 2;
	ImageOffset.y = size.y / 2;

	pos.x -= ImageOffset.x;
	pos.y += ImageOffset.y;

	glm::vec2 GeneralPivot;
	Transform ParentsTransform;

	//========================================================


	//========================================================
	GeneralPivot = pos + pivot;

	glm::mat4 model = glm::mat4(1.0f);

	model = glm::translate(model, glm::vec3(GeneralPivot, (float)m_RenderLayer)); // position

	model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * -size.y, 0.0f)); // Position
	model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotation
	model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * -size.y, 0.0f));

	model = glm::scale(model, glm::vec3(size.x * scale, size.y * scale, 1.0f));

	m_LocalModel = model;

	if (m_Parent) {
		m_Parent->ValidateDirtyTransforms();
		m_WorldMatrix = m_Parent->GetWorldModelMat() * m_LocalModel;
	}
	else {
		m_WorldMatrix = m_LocalModel;
	}

	m_IsDirty = false;
}
