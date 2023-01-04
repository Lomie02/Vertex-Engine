#include "Collider.h"

Collider::Collider()
{
	m_Vertices.push_back(glm::vec2(0.0f, 1.0f));

	m_Vertices.push_back(glm::vec2(1.0f, 0.0f));

	m_Vertices.push_back(glm::vec2(0.0f, 0.0f));

	m_Vertices.push_back(glm::vec2(0.0f, 1.0f));

	m_Vertices.push_back(glm::vec2(1.0f, 1.0f));

	m_Vertices.push_back(glm::vec2(1.0f, 0.0f));
}

Collider::Collider(ColliderType _collisionType)
{
	m_CollisionType = _collisionType;

	m_Vertices.push_back(glm::vec2(0.0f, 1.0f));

	m_Vertices.push_back(glm::vec2(1.0f, 0.0f));

	m_Vertices.push_back(glm::vec2(0.0f, 0.0f));

	m_Vertices.push_back(glm::vec2(0.0f, 1.0f));

	m_Vertices.push_back(glm::vec2(1.0f, 1.0f));

	m_Vertices.push_back(glm::vec2(1.0f, 0.0f));
}

void Collider::TranslateSpace(float x, float y)
{
	tX = x;
	tY = y;
}
