#pragma once
#include "glm.hpp"
#include "GameObject.h"
class RigidBody : public GameObject
{
public:
	RigidBody(const char* _Name);
	~RigidBody();

	void ApplyForce(glm::vec2 _force);
	void ApplyForceToActor(RigidBody* _body1, glm::vec2 _force);

	void ResolveCollision(RigidBody* _body1);
	void SetVelocity(glm::vec2 _index);
	void SetPosition(glm::vec2 _pos);

	glm::vec2 GetPosition() {return transform.position;}

	float GetMass() { return m_Mass; }
	float GetOrientation() { return m_Orientation; }
	glm::vec2 m_Velocity;

protected:
	float m_Mass;
	float m_Orientation;
};

