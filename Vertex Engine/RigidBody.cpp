#include "RigidBody.h"

RigidBody::RigidBody(const char* _Name) : GameObject(_Name)
{
	m_Mass = 1;
	m_Velocity = glm::vec2(0, -1);
}

RigidBody::~RigidBody()
{
}

void RigidBody::ApplyForce(glm::vec2 _force)
{
	m_Velocity += _force / m_Mass;
}

void RigidBody::ApplyForceToActor(RigidBody* _body1, glm::vec2 _force)
{
	_body1->m_Velocity += _force;
}

void RigidBody::ResolveCollision(RigidBody* _body1)
{
	glm::vec2 normal = glm::normalize(_body1->GetPosition() - transform.position);
	glm::vec2 releativeVelocity = _body1->m_Velocity - m_Velocity;

	float elasticity = 1;
	float j = glm::dot(-(1 + elasticity) * (releativeVelocity), normal / ((1 / m_Mass) + (1 / _body1->GetMass())));

	glm::vec2 force = normal * j;

	ApplyForceToActor(_body1, -force);
}

void RigidBody::SetVelocity(glm::vec2 _index)
{
	m_Velocity = _index;
}

void RigidBody::SetPosition(glm::vec2 _pos)
{
	transform.position = _pos;
}
