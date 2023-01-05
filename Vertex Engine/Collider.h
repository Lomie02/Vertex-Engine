#pragma once
#include <vector>
#include "glm.hpp"
enum ColliderType {
	AABB = 0,
	Circle,
	Convex,
};

class Collider
{

public:
	float tX;
	float tY;
	std::vector<glm::vec2> m_Vertices;
	
	Collider();
	Collider(ColliderType _collisionType);

	void TranslateSpace(float x, float y);


	ColliderType GetType() { return m_CollisionType; }

	float ColliderSizeCircle() { if (m_CollisionType == Circle) { return m_CircleSize; } return 0.0f; };
private:
	float m_CircleSize = 2.0f;
	ColliderType m_CollisionType = AABB;
};