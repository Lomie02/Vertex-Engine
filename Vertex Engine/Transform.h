#pragma once
#include "glm.hpp"
#include <vector>
#include "VertexComponent.h"
class Transform : public VertexComponent {

public:
	glm::vec2 position;
	glm::vec2 pivot;

	glm::vec2 PreviousPosition;

	// Local
	glm::vec2 localPosition;
	
	glm::vec2 size;
	float scale = 1.0f;
	float localScale = 0.5f;
	
	float rotation;
	float localRotation;

	void SetSize(float x, float y);
	glm::vec2 GetSize() {return size;}

	void Reset();

	glm::vec2 GetCenter();
	float GetSizeFromCentre(glm::vec2 _axis);

	void ClearTransforms() { position = glm::vec2(0, 0); rotation = 0.0f; pivot = glm::vec2(0, 0); }
	void ApplyTransform() { position += pivot; pivot = glm::vec2(0, 0); } // Test feature.

	void ApplyMatrix(glm::mat4 _mat) { m_LocalModel = _mat; }

	void SetLayer(int _layer) { m_RenderLayer = _layer; }

	//==================================== New Transform Data
	glm::mat4 GetModelMatrixEditable() { return m_TransformModelMatrix; }

	glm::mat4 GetLocalModelMat() ;
	glm::mat4 m_TransformModelMatrix = glm::mat4(1.0f);

	void RenderEditorDisplay() override;


	glm::mat4 GetWorldModelMat()  {

		if (HasChanged()) ValidateDirtyTransforms();

		if (m_Parent) {
			
			m_WorldMatrix = m_Parent->GetWorldModelMat() * m_LocalModel;
			return m_WorldMatrix;
		}
		else {

			m_WorldMatrix = m_LocalModel;
			return m_WorldMatrix;
		}
	}

	Transform* GetParent() { return m_Parent; }

	void SetParent(Transform* _parent);
	void RemoveParent();
	void SetChild(Transform* _child);
	void RemoveChild(Transform* _child);

	bool IsChildOf(const Transform* _child) const;
	void LateUpdate(float delta) override;
	std::vector<Transform*> GetChildren() { return m_Children; }

private:

	bool HasChanged();
	void ValidateDirtyTransforms(bool _forceValidate = false);

	int m_RenderLayer;

	Transform* m_Parent;
	std::vector<Transform*> m_Children;

	glm::vec2 m_LastPosition;
	glm::vec2 m_LastSize;
	float m_LastScale;
	float m_LastRotation;

	bool m_IsDirty = false;
	glm::mat4 m_LocalModel = glm::mat4(1.0f);
	glm::mat4 m_WorldMatrix = glm::mat4(1.0f);
};