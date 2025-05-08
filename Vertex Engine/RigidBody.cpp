#include "RigidBody.h"

RigidBody::RigidBody(const char* _Name)
{
	transform.SetSize(5,5);
}

RigidBody::~RigidBody()
{
	delete m_Body;
	m_Body = nullptr;

	delete m_Motion;
	m_Motion = nullptr;

}

void RigidBody::Init(btCollisionShape* _shape, float _mass)
{
	this->m_ShapeData = _shape;
	btVector3 inetria(0, 0, 0);

	m_ShapeData->setMargin(0.0f);
	m_ShapeData->calculateLocalInertia(m_Mass, inetria);

	btTransform tran;
	tran.setIdentity();
	tran.setOrigin(btVector3(transform.position.x, transform.position.y, 0));

	m_Motion = new btDefaultMotionState(tran);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(m_Mass, m_Motion, m_ShapeData, inetria);
	m_Body = new btRigidBody(rbInfo);

	m_Body->setAngularFactor(btVector3(0, 0, 1));
	SetFriction(0.2f);
	m_Body->setUserPointer(this);
}

void RigidBody::ApplyForce(glm::vec3 _force)
{
	m_Body->applyCentralForce(btVector3(_force.x, _force.y, _force.z));
}

void RigidBody::ApplyTorque(glm::vec3 _torq)
{
	m_Body->applyTorque(btVector3(_torq.x, _torq.y, _torq.z));
}

void RigidBody::SetFriction(float _friction)
{
	m_Body->setFriction(_friction);
}

glm::vec3 RigidBody::GetForward()
{
	btTransform trans;
	m_Body->getMotionState()->getWorldTransform(trans);
	btQuaternion rotation = trans.getRotation();

	btVector3 local(0, 1, 0);
	btVector3 worldForward = quatRotate(rotation, local);

	return glm::vec3(worldForward.x(), worldForward.y(), worldForward.z());
}

void RigidBody::SetKimatics(bool _state)
{
	if (_state) {
		m_Body->setCollisionFlags(m_Body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
		m_Body->setActivationState(DISABLE_DEACTIVATION);
	}
	else {
		m_Body->setCollisionFlags(m_Body->getCollisionFlags() | ~btCollisionObject::CF_KINEMATIC_OBJECT);
		m_Body->setActivationState(ACTIVE_TAG);

		btVector3 inertia(0, 0, 0);
		m_Body->getCollisionShape()->calculateLocalInertia(m_Mass, inertia);
		m_Body->setMassProps(m_Mass, inertia);
		m_Body->updateInertiaTensor();
	}

	m_IsKimatic = _state;
}
