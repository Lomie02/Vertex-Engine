#include "VertexTransitions.h"
/*
	NOTE:
		This is the test version for the Vertex Transition screens & will be updated in the future to support more transition images ontop of 1 gameobject.
*/
VertexTransitions::VertexTransitions() : VertexScene("DEFAULT_SCENE_VERTEX")
{
	Awake();
}

VertexTransitions::VertexTransitions(const char _name[]) : VertexScene(_name)
{
	Awake();
}

VertexTransitions::~VertexTransitions()
{
	delete m_TransitionObject;
	m_TransitionObject = nullptr;

	delete m_MainCamera;
	m_MainCamera = nullptr;
}


void VertexTransitions::TransitionSpeed(float _amount)
{
	m_TransitionSpeed = _amount;
	if (_amount < 0.5) {
		m_TransitionSpeed = 0.5;
	}
}

void VertexTransitions::Awake()
{
	m_TransitionObject = new GameObject("Object");

	m_Transitions.push_back(m_TransitionObject);

	ResourceManager::LoadTexture("Builds/Textures/VertexMainDisplay.png", "VertexMainDisplay");

	m_Transitions.at(0)->material.AlbedoMap = ResourceManager::GetTexture("VertexMainDisplay");
	m_Transitions.at(0)->material.surface = Transparent;
	m_TransitionSpeed = 2;

	m_MainCamera = new Camera("Transition Camera");

	m_Manager.Register(m_MainCamera);
	m_Timer = m_TransitionSpeed;

	m_Manager.Register(m_TransitionObject);

	m_TransitionObject->transform.size.x = 1920;
	m_TransitionObject->transform.size.y = 1080;

	m_MainCamera->zoom = 0.002f;
}

void VertexTransitions::Start()
{
	m_PPA.ChromaticAberation.ChromaticEnabled = false;
	m_Manager.Register(m_PPA);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void VertexTransitions::Update(float delta)
{
	if (!m_LerpColour) {

		m_Timer -= 0.5f * delta;

		if (m_Timer <= 0) {

			m_LerpColour = true;
		}
	}

	if (m_LerpColour && m_Transitions.at(0)->material.colour.a >= 0)
	{
		m_Transitions.at(0)->material.colour.a -= m_FadeDuration * delta;

		if (m_Transitions.at(0)->material.colour.a < 0)
		{
			m_Timer = m_TransitionSpeed;
			m_SceneManager->SetActiveScene(1);
			m_LerpColour = false;
			m_Transitions.at(0)->material.colour.a = 1;
		}
	}

	if (Input::GetKeyDown(m_Window, GLFW_KEY_SPACE) && !m_LerpColour) {
		m_LerpColour = true;
	}
}

void VertexTransitions::FixedUpdate(float fixedDelta)
{
}

void VertexTransitions::LateUpdate(float delta)
{
}

void VertexTransitions::Rendering(Vertex2D* renderer)
{
	m_Manager.ConfigureRenderSystems(renderer);
}

Material VertexTransitions::EditTransition(int _index)
{
	if (_index != 0 && _index <= m_Transitions.size())
	{
		return m_Transitions.at(_index)->material;
	}

	// Returns an empty material to avoid crashing the game once it starts,
	Material empty;
	return empty;
}
