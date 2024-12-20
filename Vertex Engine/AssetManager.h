#pragma once
#include "GameObject.h"
#include <vector>
#include "VertexRenderPackage.h"
#include "VertexUIPackage.h"

#include <type_traits>
#include "Camera.h"
#include "Animator.h"
#include "BootUpContainer.h"
#include "RigidBody.h"

#include "VertexGeneral.h"
#include <irrKlang.h>
#include "Mime.h"
#include "NavAgent.h"
#include "irrKlang-64bit-1.6.0/include/irrKlang.h"
#include "Canvas.h"

using namespace irrklang;

enum Renderer {
	Vertex_2D = 0, // Vertex Engines default renderer.
	Tension_2D, //  Tension 2D is a more advanced version of Vertex 2D
};

/// <summary>
/// The backbone of the engine is the Asset Manager. Each scene has their own asset manager that controls every aspect of the scene.
/// </summary>
class AssetManager
{
	friend class SceneManager;
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

	/// <summary>
	/// The Template Vesions
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="_object"></param>

	template <typename T>
	void Register(T) {
		switch (sizeof(T)) {
			case sizeof(GameObject*) :
				std::cout << "The variable is of type GameObject.\n";
				break;
		}
	}

	// Register Functions
	void Register(VertexComponent* _object); // Componenet Systens
	//void Register(GameObject * _object);// Normal Objects
	void Register(RigidBody* _object); // Physics object
	void Register(Animator* _object); // TODO switch the animator to be a component
	void Register(Button* _object); // Button

	void Register(Camera* camera); // Cameras
	void Register(Text* _text); // Text objects
	void Register(Volume& _text); // Volumes Post Processing
	void Register(NavAgent* _nav); // AI 
	void Register(AudioSource* _audio); // Audio
	void Register(Canvas* _canvas); // Register Canvas

	void GiveWindow(GLFWwindow* _window) { m_Window = _window; }

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

	void RegisterUserInterfaceCamera(Camera* _camera) { m_Vertex_Ui_Camera = _camera; }
	void SetCanvasDisplayActive(int _index) { m_ActiveCanvasDisplay = _index; }
	void GiveSceneManager(SceneManager* _sceneManagerPtr) { m_SceneManager = _sceneManagerPtr; std::cout << "Scene Manager Given." << std::endl; }

	SceneManager* GetSceneManager() { return m_SceneManager; }
private:

	Camera* m_Vertex_Ui_Camera;
	std::vector<AudioSource*> m_AudioSources;
	ISoundEngine* m_SoundEngine;
	TransparencySorting m_SortingTransparentAlgo;
	Volume m_SceneVolume;
	SceneManager* m_SceneManager;

	std::vector<Canvas*> m_CanvasList;
	int m_ActiveCanvasDisplay = 0;

	std::vector<VertexComponent*> m_VertexComponentsList;
	std::vector<GameObject*> m_TransParentList;
	std::vector<NavAgent*> m_NavList;
	EditorMode m_OperatingMode;

	char m_ScenesName[30] = "s";
	irrklang::ISoundEngine* m_SoundSystem;
	std::vector<GameObject*> m_Opaque;
	std::vector<GameObject*> m_Transparent;

	Renderer m_RendererToUse = Tension_2D;
	bool m_SingleSortRenderering = true;
	bool m_HasRendered = false;

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

	// Asset private functions
	void AssignSoundEngineToVertexComponenets();
	void SwapResources(std::vector<GameObject*> _list, int _element1, int _element2);
	int Partition(std::vector<GameObject*> _list, int _start, int _end);
	void ConfigureMouse();

	void InsertionSort();
	void QuickSort(std::vector<GameObject*> _list, int _start, int _end);
	void UpdateComponents(float delta);
	void CollisionCheck();
	bool OnTrigger(GameObject* A, GameObject* B);
};