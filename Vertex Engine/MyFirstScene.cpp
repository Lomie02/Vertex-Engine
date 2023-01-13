#include "MyFirstScene.h"

MyFirstScene::MyFirstScene()
{
	Awake();
}

MyFirstScene::MyFirstScene(const char _name[]) : VertexScene(_name)
{
	Awake();
}

MyFirstScene::~MyFirstScene()
{
	delete m_MyCamera;
	m_MyCamera = nullptr;
}

void MyFirstScene::Awake()
{
	m_MyCamera = new Camera("My Camera");

	m_Manager.Register(m_MyCamera);

	m_GameObject = new GameObject("Object!");

	m_Manager.Register(m_MyCamera);

}

void MyFirstScene::Start()
{
}

void MyFirstScene::Update(float delta)
{
	m_Manager.LogEvents();

}

void MyFirstScene::LateUpdate(float delta)
{
}

void MyFirstScene::FixedUpdate(float fixedDelta)
{
}

void MyFirstScene::Rendering(Vertex2D* renderer)
{
}
