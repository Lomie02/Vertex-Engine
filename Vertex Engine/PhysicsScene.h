#pragma once
#include <vector>
#include "GameObject.h"
#include "bullet/btBulletDynamicsCommon.h"

/*
	TODO: Implement the physics scenes into the asset manager. Physics scenes will deal with all collisions and physics calulations.
	The rest of the class will be implemented into the engine over time once a plan has been layed out for how the system should behave and work.
*/

class PhysicsScene
{
public:
	PhysicsScene();
	~PhysicsScene();

	void FixedUpdate(float fixedDelta);
	void Register(GameObject* _objects);

	void SetGravity(glm::vec3 _gravity);



private:
	btCollisionDispatcher* m_Dispatcher;
	btDefaultCollisionConfiguration* m_Config;
	btSequentialImpulseConstraintSolver* m_Solver;

	btBroadphaseInterface* m_BroadPhase;
	btDiscreteDynamicsWorld* m_World;

};

