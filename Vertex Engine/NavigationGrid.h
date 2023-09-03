#pragma once
#include "Transform.h"
#include <list>
#include <vector>
struct Edge;

struct NavigationNode {
	NavigationNode* m_Parent;
	glm::vec2 transform;
	int m_GScore;

	std::list<Edge> m_Connections;
	bool m_IsEnabled;

	NavigationNode() : m_GScore(0), transform(0,0), m_Parent(nullptr), m_IsEnabled(NULL) {};
	NavigationNode(glm::vec2 _position) : m_GScore(0), transform(_position), m_Parent(nullptr), m_IsEnabled(NULL) {};
	NavigationNode(glm::vec2 _position, NavigationNode* _node) : m_GScore(0), transform(_position), m_Parent(_node), m_IsEnabled(NULL) {};
};

struct Edge {
	NavigationNode* m_Connections;
	float m_Cost;
	Edge() : m_Connections(nullptr), m_Cost(0){}
	Edge(NavigationNode* _node) : m_Connections(_node), m_Cost(0){}
	Edge(NavigationNode* _node, float _cost) : m_Connections(_node), m_Cost(_cost){}
};
class NavigationGrid
{
public:
	NavigationGrid();
	~NavigationGrid();

	NavigationNode* AddNode(glm::vec2 _pos);
	void AddConnection(NavigationNode* _a, NavigationNode* _b, float _cost = 1.0f);
	void NavigatePathSearch(NavigationNode* _start, NavigationNode* _end, std::list<NavigationNode*> _foundPath);

	NavigationNode* GetNode(int _index);
	NavigationNode* GetNodePosition(glm::vec2 _pos);
protected:
	std::vector<NavigationNode*> m_Nodes;
};

