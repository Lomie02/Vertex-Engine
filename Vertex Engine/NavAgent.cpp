#include "NavAgent.h"

NavAgent::NavAgent(NavigationGrid* graph)
{
	m_PathFindingSystem = graph;
}

NavAgent::~NavAgent()
{
}

void NavAgent::UpdateSystem(float delta)
{
	if (!m_IsStopped) {


		m_TimeAlongPath += delta * 0.1;
		m_TimeAlongPath = m_TimeAlongPath > 1 ? m_TimeAlongPath - 1 : m_TimeAlongPath;

		if (m_Path.size() < 1) {

			m_StartNode = m_PathFindingSystem->GetNodePosition(transform.position);
			m_EndNode = m_PathFindingSystem->GetNode(5);
			m_PathFindingSystem->NavigatePathSearch(m_StartNode, m_EndNode, m_Path);
			m_TimeAlongPath = 0;
		}

		if (m_Path.size() > 1) {

			int node = m_TimeAlongPath * m_Path.size();
			float TimeThroughNode = m_TimeAlongPath * m_Path.size() - 0.01 - node;
			auto elem = m_Path.begin();

			for (int i = 0; i < node; i++)
			{
				elem = std::next(elem);
			}

			auto next = std::next(elem);
			if (next == m_Path.end())
			{
				m_StartNode = m_EndNode;
				m_Path.clear();
			}
			else
			{
				transform.position = (*elem)->transform * (1 - TimeThroughNode) + (*next)->transform * (TimeThroughNode);
			}
		}
	}
}

void NavAgent::SetDestination(glm::vec2 _position)
{
}

NavigationNode* NavAgent::GetStart()
{
	return m_StartNode;
}

NavigationNode* NavAgent::GetEnd()
{
	return m_EndNode;
}
