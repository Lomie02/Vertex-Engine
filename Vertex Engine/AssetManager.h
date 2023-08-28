#pragma once
#include "GameObject.h"
#include <vector>
#include "VertexRenderPackage.h"
#include "VertexUIPackage.h"
#include "Camera.h"
#include "Animator.h"
#include "BootUpContainer.h"
#include "RigidBody.h"

#include "VertexGeneral.h"
#include <irrKlang.h>
#include "Mime.h"

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

	~AssetManager();

	void AssignSoundSystem(irrklang::ISoundEngine* _engine);
	void BootUpAll(BootUpContainer* _settings);

	// Register Functions
	void Register(VertexComponent* _object); // Componenet Systens
	void Register(GameObject* _object);// Normal Objects
	void Register(RigidBody* _object); // Physics object
	void Register(Animator* _object); // TODO switch the animator to be a component
	void Register(Button* _object); // Button
	void Register(Camera* camera); // Cameras
	void Register(Text* _text); // Text objects
	void Register(Volume& _text); // Volumes Post Processing

	void GiveWindow(GLFWwindow* _window) { m_Window = _window; };
	bool OnTrigger(GameObject* A, GameObject* B);

	void ConfigureSystems();
	void ConfigureRenderSystems(Vertex2D* render);
	void ConfigurePhysics(float fixedDelta);
	void LogEvents();
	void SetActiveCamera(int _index);

	// For the Editor to access.
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
	void AssignMode(EditorMode _mode) { m_OperatingMode = _mode; }

	std::vector<Button*> GetButtonObjects() { return m_UiButtonObjects; }
	std::vector<Text*> GetTextObjects() { return m_UiTextObjects; }

private:

	Volume m_SceneVolume;
	void UpdateComponents(float delta);
	std::vector<VertexComponent*> m_VertexComponentsList;
	std::vector<GameObject*> m_TransParentList;
	EditorMode m_OperatingMode;

	char m_ScenesName[30] = "s";
	void ConfigureMouse();
	irrklang::ISoundEngine* m_SoundSystem;
	
	bool m_SingleSortRenderering = true;
	bool m_HasRendered = false;
	Renderer m_RendererToUse = Tension_2D;

	std::vector<GameObject*> m_Opaque;
	std::vector<GameObject*> m_Transparent;
	void CollisionCheck();

	GLFWwindow* m_Window;
	Transform mouse;
	float m_TimeStep = 0.01f;
	glm::vec2 m_WorldGravity = glm::vec2(0, -1);

	Material m_CameraGizmo;
	Material m_TransformGizmoX;
	Material m_CenterGizmo;

	bool m_DeleteAssetPointersAuto = false;

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