#include "PhysicsScene.h"

void PhysicsScene::SyncGameObjectPositions(int _pass)
{
	if (_pass == 0) { // Assignment
		for (int i = 0; i < m_2dObjectsInWorld.size(); i++) {
			btTransform trans;
			m_2dObjectsInWorld.at(i)->GetBody()->getMotionState()->getWorldTransform(trans);

			btScalar yaw, pitch, roll;
			btQuaternion rotation = trans.getRotation();

			rotation.getEulerZYX(yaw, pitch, roll);

			m_2dObjectsInWorld.at(i)->transform->position = glm::vec3(trans.getOrigin().x(), trans.getOrigin().y(), trans.getOrigin().z());
			m_2dObjectsInWorld.at(i)->transform->rotation = glm::quat(glm::vec3(rotation.x(),rotation.y(), rotation.z()));

		}
	}
	else {

	}
}

PhysicsScene::PhysicsScene()
{
	m_Config = new btDefaultCollisionConfiguration();
	m_Dispatcher = new btCollisionDispatcher(m_Config);
	m_BroadPhase = new btDbvtBroadphase;

	m_Solver = new btSequentialImpulseConstraintSolver();
	m_World = new btDiscreteDynamicsWorld(m_Dispatcher, m_BroadPhase, m_Solver, m_Config);

	SetGravity(glm::vec3(0, -1.0f, 0));

	std::cout << "Vertex Message: Bullet Created!" << std::endl;
}

PhysicsScene::~PhysicsScene()
{
	delete m_Config;
	m_Config = nullptr;

	delete m_Dispatcher;
	m_Dispatcher = nullptr;

	delete m_BroadPhase;
	m_BroadPhase = nullptr;

	delete m_Solver;
	m_Solver = nullptr;

	delete m_World;
	m_World = nullptr;
}

void PhysicsScene::FixedUpdate(float fixedDelta)
{
	if (m_World != nullptr) {
		m_World->stepSimulation(fixedDelta);
		SyncGameObjectPositions(0);
	}
}

void PhysicsScene::Register(RigidBody* _objects)
{
	m_2dObjectsInWorld.push_back(_objects);
	m_World->addRigidBody(_objects->GetBody());
}

void PhysicsScene::SetGravity(glm::vec3 _gravity)
{
	m_World->setGravity(btVector3(_gravity.x, _gravity.y, _gravity.z));

}
