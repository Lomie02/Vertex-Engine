#pragma once
#include <vector>
#include "GameObject.h"
#include "bullet/btBulletDynamicsCommon.h"
#include "RigidBody.h"


enum PhysicsCalculationMode {
	Two2 = 0,
	ThreeD,

};

class PhysicsScene
{
protected:
	/// <summary>
	/// Syncs all objects postions to match the simulations
	/// </summary>
	/// <param name="_pass"></param>
	void SyncGameObjectPositions(int _pass);

public:
	PhysicsScene();
	~PhysicsScene();

	/// <summary>
	/// Fixed Update Loop.
	/// </summary>
	/// <param name="fixedDelta"></param>
	void FixedUpdate(float fixedDelta);
	/// <summary>
	/// Register 2D rigidbodies to the physics simulation.
	/// </summary>
	/// <param name="_objects"></param>
	void Register(RigidBody* _objects);
	/// <summary>
	/// Register 3D Objects rigidbody componenet.
	/// </summary>
	/// <param name="_objects"></param>
	void Register(VertexComponent* _objects);

	/// <summary>
	/// Sets the gravity of the world.
	/// </summary>
	/// <param name="_gravity"></param>
	void SetGravity(glm::vec3 _gravity);

	/// <summary>
	/// Returns a list of all 2D objects in the world.
	/// </summary>
	/// <returns></returns>
	std::vector<RigidBody*> Get2dObjects() { return m_2dObjectsInWorld; }

private:
	PhysicsCalculationMode m_CalcMode;

	int m_SyncPassStage = 0;

	btCollisionDispatcher* m_Dispatcher;
	btDefaultCollisionConfiguration* m_Config;
	btSequentialImpulseConstraintSolver* m_Solver;

	btBroadphaseInterface* m_BroadPhase;
	btDiscreteDynamicsWorld* m_World;

	std::vector<RigidBody*> m_2dObjectsInWorld;
	std::vector<VertexComponent*> m_3dObjectsInWorld;
};

