#pragma once
#include "glm.hpp"

class Transform {
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

	glm::mat4 GetGlobalMatrix() {  return m_GlobalTransforms; }
	glm::mat4 GetLocalMatrix() { return m_LocalTransforms; }

	void ApplyGlobalMatrix(glm::mat4 _mat) { m_GlobalTransforms = _mat; }
	void ApplyLocalMatrix(glm::mat4 _mat) { m_LocalTransforms = _mat; }

private:
	glm::mat4 m_GlobalTransforms = glm::mat4(1);
	glm::mat4 m_LocalTransforms = glm::mat4(1);
};