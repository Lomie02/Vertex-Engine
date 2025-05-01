#include "PhysicsScene.h"

PhysicsScene::PhysicsScene()
{
	m_Config = new btDefaultCollisionConfiguration();
	m_Dispatcher = new btCollisionDispatcher(m_Config);
	m_BroadPhase = new btDbvtBroadphase();

	m_Solver = new btSequentialImpulseConstraintSolver();
	m_World = new btDiscreteDynamicsWorld(m_Dispatcher, m_BroadPhase, m_Solver, m_Config);

	SetGravity(glm::vec3(0, -9.1f, 0));

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
	m_World->stepSimulation(fixedDelta);
}

void PhysicsScene::SetGravity(glm::vec3 _gravity)
{
	m_World->setGravity(btVector3(_gravity.x, _gravity.y, _gravity.z));
}
