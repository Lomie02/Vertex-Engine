#pragma once
#include "GameObject.h"
#include <vector>
#include "VertexRenderPackage.h"
#include "VertexUIPackage.h"
#include "Camera.h"
#include "Animator.h"
#include "BootUpContainer.h"
//#include "SceneManager.h"
#include "RigidBody.h"

enum Renderer {
	Vertex_2D = 0, // Vertex Engines default renderer.
	Tension_2D, //  Tension is an upcoming Renderer that supports more features than the regular Vertex2D Renderer
};

class AssetManager
{
public:

	AssetManager()
	{
		m_ActiveCamera = 0;
		m_CameraGizmo.baseTexture = ResourceManager::GetTexture("Gizmo_Camera");
		m_TransformGizmoX.baseTexture = ResourceManager::GetTexture("Gizmo_Cords");
		m_CenterGizmo.baseTexture = ResourceManager::GetTexture("Gizmo_Center");
	};

	void BootUpAll(BootUpContainer* _settings);

	void Register(GameObject* _object);
	void Register(RigidBody* _object);
	void Register(Animator* _object);
	void Register(Button* _object);
	void Register(Camera* camera);
	void Register(Text* _text);

	void GiveWindow(GLFWwindow* _window) { m_Window = _window; };
	bool OnTrigger(GameObject* A, GameObject* B);

	bool CollisionCheck();
	void ConfigureSystems();
	void ConfigureRenderSystems(Vertex2D* render);
	void ConfigurePhysics(float fixedDelta);
	void LogEvents();
	void SetActiveCamera(int _index);

	std::vector<GameObject*> m_Objects;
	std::vector<GameObject*> m_UiObjects;
	std::vector<Camera*> m_Cameras;
	std::vector<Animator*> m_Animators;

	bool MousePick(GameObject* _target);
	bool Raycast2D(glm::vec2 _pos, glm::vec2 _dir, GameObject& _out, float length = 100.0f);

	glm::vec2 GetMousePosition() { ConfigureMouse(); return mouse.position; }

	// Tension 2D Functions
	void TensionRendering(Vertex2D* m_Renderer);
	void TensionLayerSort();

	void Vertex2dRendering(Vertex2D* render);

	void ConfigSetup();
	void ExecuteAll();
	void UnRegister(GameObject* _target);
	void Gizmos(Vertex2D* render);

	GameObject* FindObjectWithTag(std::string _tag);
	std::vector<GameObject*> FindObjecstWithTag(std::string _tag);
	std::vector<GameObject*> FindObjecstWithComponent(VertexComponent& _ref);
	GameObject* FindObjectWithComponent(VertexComponent& _ref);
	GameObject* FindComponent(VertexComponent& _ref);

	GLFWwindow* GetWindow() { return m_Window; }
	//void AssignSceneManager(SceneManager* _scene) { SceneManager = _scene; }
private:

	//SceneManager* SceneManager;
	char m_ScenesName[30] = "s";
	void UpdateComponents(float delta);
	void ConfigureMouse();

	bool m_SingleSortRenderering = true;
	bool m_HasRendered = false;
	Renderer m_RendererToUse = Tension_2D;

	std::vector<GameObject*> m_Opaque;
	std::vector<GameObject*> m_Transparent;

	GLFWwindow* m_Window;
	Transform mouse;
	float m_TimeStep = 0.01f;
	glm::vec2 m_WorldGravity = glm::vec2(0, -1);

	Material m_CameraGizmo;
	Material m_TransformGizmoX;
	Material m_CenterGizmo;

	std::vector<RigidBody*> m_PhysicsObjects;
	std::vector<Transform*> m_PreviousLocations;
	std::vector<Button*> m_UiButtonObjects;
	std::vector<Text*> m_UiTextObjects;
	int m_ActiveCamera;
	bool m_ShutDownManager = false;

	float lastX = 1920;
	float lastY = 1080;

	float yaw = 0;
	float pitch = 0;

};