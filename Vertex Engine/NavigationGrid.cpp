#include "NavigationGrid.h"


bool SortAscene(NavigationNode* _a, NavigationNode* _b) {
	return _a->m_GScore < _b->m_GScore;
}

NavigationGrid::NavigationGrid()
{
}

NavigationGrid::~NavigationGrid()
{
	for (auto cast = m_Nodes.begin(); cast != m_Nodes.end(); cast++) {
		delete(*cast);
	}
	m_Nodes.clear();
}

NavigationNode* NavigationGrid::AddNode(glm::vec2 _pos)
{
	m_Nodes.push_back(new NavigationNode(_pos));
	return m_Nodes.back();
}

void NavigationGrid::AddConnection(NavigationNode* _a, NavigationNode* _b, float _cost)
{
	if (_a->m_IsEnabled == false || _b->m_IsEnabled == false)
		return;
	_a->m_Connections.push_back(Edge(_b, _cost));
}

void NavigationGrid::NavigatePathSearch(NavigationNode* _start, NavigationNode* _end, std::list<NavigationNode*> _foundPath)
{
	_foundPath.clear();

	if (!_start->m_IsEnabled)
		return;

	if (!_end->m_IsEnabled)
		return;

	int _currentStep = 0;
	std::list<NavigationNode* > OpenList;
	std::list<NavigationNode* > ClosedList;

	_start->m_GScore = 0;
	_start->m_Parent = nullptr;

	OpenList.push_back(_start);

	while (!OpenList.empty()) {
		OpenList.sort(SortAscene);

		NavigationNode* node = OpenList.front();
		OpenList.pop_front();

		ClosedList.push_back(node);

		for (auto it = node->m_Connections.begin(); it != node->m_Connections.end(); it++)
		{
			NavigationNode* connections = (*it).m_Connections;
			if (connections == nullptr) {
				continue;
			}
			if (!connections->m_IsEnabled) {
				continue;
			}

			bool InOpen = std::find(OpenList.begin(), OpenList.end(), connections) != OpenList.end();
			bool InClosed = std::find(ClosedList.begin(), ClosedList.end(), connections) != ClosedList.end();

			float gScore = node->m_GScore + (*it).m_Cost;

			if (!InClosed) {
				connections->m_GScore = gScore;
				connections->m_Parent = node;
			}
			else {
				if (gScore < connections->m_GScore) {
					connections->m_GScore = gScore;
					connections->m_Parent = node;
				}
			}
			if (!InOpen && !InClosed) {
				OpenList.push_back(connections);
			}

		}
	}

	NavigationNode* current = _end;
	while (current != nullptr) {
		_foundPath.insert(_foundPath.begin(), current);
		current = current->m_Parent;
	}

}

NavigationNode* NavigationGrid::GetNode(int _index)
{
	return m_Nodes[_index];
}

NavigationNode* NavigationGrid::GetNodePosition(glm::vec2 _pos) //TODO: I dont know if this will be needed or not. seems unlikely,
{
	int indexX;
	int indexY;

	indexX = (_pos.x + 40 / 2 - 100) / 40;
	indexY = (_pos.y + 40 / 2 - 100) / 40;

	return m_Nodes[indexY * 11 + indexX];
}
