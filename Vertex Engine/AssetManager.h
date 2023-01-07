#pragma once
#include "GameObject.h"
#include <vector>
#include "VertexRenderPackage.h"
#include "VertexUIPackage.h"
#include "Camera.h"
#include "Animator.h"

#include "RigidBody.h"

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
	void Register(GameObject* _object);
	void Register(RigidBody* _object);
	void Register(Animator* _object);
	void RegisterUi(GameObject* _object);
	void RegisterUi(Button* _object);
	void Register(Camera* camera);

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

	void ConfigSetup();
	void ExecuteAll();
	void UnRegister(GameObject* _target);
	void Gizmos(Vertex2D* render);

private:
	void UpdateComponents(float delta);
	void ConfigureMouse();

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