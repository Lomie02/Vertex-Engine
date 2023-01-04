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
	Collider();
	Collider(ColliderType _collisionType);

	void TranslateSpace(float x, float y);

	float tX;
	float tY;

	ColliderType GetType() { return m_CollisionType; }
	std::vector<glm::vec2> m_Vertices;
private:
	ColliderType m_CollisionType = AABB;
};