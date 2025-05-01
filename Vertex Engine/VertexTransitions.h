#include "VertexScene.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "GameObject.h"
class VertexTransitions : public VertexScene

{
public:
	VertexTransitions();
	VertexTransitions(const char _name[]);
	~VertexTransitions();

	void GiveWindow(GLFWwindow* _window) { m_Window = _window; }
	void GiveWindow(SceneManager* _scene) { m_SceneManager = _scene; }

	void TransitionSpeed(float _amount);

	virtual void Awake();
	virtual void Start();
	virtual void Update(float delta);
	virtual void FixedUpdate(float fixedDelta);
	virtual void LateUpdate(float delta);
	virtual void Rendering(Vertex2D* renderer);

	Material EditTransition(int _index);
private:
	float m_TransitionSpeed = 2;
	GLFWwindow* m_Window;
	SceneManager* m_SceneManager;

	float m_FadeDuration = 0.4f;
	GameObject* m_TransitionObject;
	float m_Timer = 0;
	int m_CurrentDisplay;
	std::vector<GameObject*> m_Transitions;

	Volume m_PPA;

	Camera* m_MainCamera;
	bool m_LerpColour;
};