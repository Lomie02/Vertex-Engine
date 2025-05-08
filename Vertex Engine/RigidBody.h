#pragma once
#include "glm.hpp"
#include "GameObject.h"
#include "bullet/btBulletDynamicsCommon.h"

class RigidBody : public GameObject
{
public:
	RigidBody(const char* _Name);
	~RigidBody();

	void Init(btCollisionShape* _shape, float _mass);
	void ApplyForce(glm::vec3 _force);
	
	btRigidBody* GetBody() { return m_Body; }

	void SetMass(float _mass) { m_Mass = _mass; }
	float GetMass() { return m_Mass; }

	void ApplyTorque(glm::vec3 _torq);

	btCollisionShape* GetCollider() { return m_ShapeData; }

	void SetFriction(float _friction);
	glm::vec3 GetForward();

	void SetKimatics(bool _state);
	bool IsKimatic() { return m_IsKimatic; }

protected:

	bool m_IsKimatic = false;
	btRigidBody* m_Body;
	glm::vec3 m_Forward;

	btMotionState* m_Motion;
	btCollisionShape* m_ShapeData;

	float m_Mass = 1;
	float m_Orientation;
};

