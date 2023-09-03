#pragma once
#include "GameObject.h"
#include "NavigationGrid.h"

class NavAgent : public GameObject
{
public:
	NavAgent(NavigationGrid* graph);
	~NavAgent();

	void UpdateSystem(float delta);
	void SetDestination(glm::vec2 _position);
	NavigationNode* GetStart();
	NavigationNode* GetEnd();

	void Stop() { m_IsStopped = true; };
	void Resume() { m_IsStopped = false; };
	bool HasReachedEnd() {return m_IsEnded;}

private:
	std::list<NavigationNode*> m_Path;
	std::list<NavigationNode*> m_EndNodes;
	bool m_IsEnded = false;
	NavigationNode* m_StartNode;
	NavigationNode* m_EndNode;

	bool m_IsStopped = true;
	NavigationGrid* m_PathFindingSystem;

	float m_TimeAlongPath = 0;
};

