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
#include "irrKlang-64bit-1.6.0/include/irrKlang.h"
#include "Canvas.h"
#include "glm.hpp"
#include "PhysicsScene.h"
using namespace irrklang;

#include "vGameObject.h"
#include <unordered_map>

enum Renderer {
	Vertex_2D = 0, // Vertex Engines default renderer.
	Tension_2D, //  Tension 2D is a more advanced version of Vertex 2D
	Violet_3D,
};

/// <summary>
/// The backbone of the engine is the Asset Manager. Each scene has their own asset manager that controls every aspect of the scene.
/// </summary>
/// 
/// 


class AssetManager
{
	friend class SceneManager;
	friend class VertexEngineEditor;
public:

	AssetManager()
	{
		m_EditorCamera = new GameObject("EditorCamera");

		m_UiRenderTexture = new RenderTexture(PROJECT_ASPECT_WIDTH, PROJECT_ASPECT_HEIGHT);
		m_EditorColourPicker = new RenderTexture(PROJECT_ASPECT_WIDTH, PROJECT_ASPECT_HEIGHT);
		m_EditorCamera->AddComponent<Camera>();
		m_EditorCamera->GetComponenet<Camera>()->renderTexture = m_EditorColourPicker;
		m_EditorCamera->GetComponenet<Camera>()->SetDisplay(8);
		m_EditorCamera->GetComponenet<Camera>()->zoom = 20;

		m_ActiveCamera = 0;
		m_CameraGizmo.AlbedoMap = ResourceManager::GetTexture("Gizmo_Camera");
		m_TransformGizmoX.AlbedoMap = ResourceManager::GetTexture("Gizmo_Cords");
		m_CenterGizmo.AlbedoMap = ResourceManager::GetTexture("Gizmo_Center");

		m_PhysicsScene = new PhysicsScene();
	};

	~AssetManager();

	void AssignSoundSystem(irrklang::ISoundEngine* _engine);
	void BootUpAll(BootUpContainer* _settings);

	void SetWorldGravity(glm::vec3 _grav) { m_PhysicsScene->SetGravity(_grav); }
	// Register Functions
	void Register(VertexComponent* _object); // Componenet Systens
	void Register(GameObject* _object);// Normal Objects
	void Register(RigidBody* _object); // Physics object
	void Register(Animator* _object); // TODO switch the animator to be a component
	void Register(Button* _object); // Button

	void Register(Camera* camera); // Cameras
	void Register(Text* _text); // Text objects
	void Register(Volume& _text); // Volumes Post Processing
	void Register(AudioSource* _audio); // Audio
	void Register(Canvas* _canvas); // Register Canvas

	void Register(vGameObject* _object);

	GameObject* RegisterGameObjectNew(GameObject* _parent = nullptr, GameObject* _child = nullptr);

	//========================================================================

	void GiveWindow(GLFWwindow* _window) { m_Window = _window; }

	void ConfigureSystems();
	void ConfigureRenderSystems(Vertex2D* render);
	void ConfigurePhysics(float fixedDelta);
	void LogEvents();
	void SetActiveCamera(int _index);

	//========================================================================
	// For the Editor to access.
	std::vector<GameObject*> m_Objects;
	std::vector<GameObject*> m_UiObjects;
	std::vector<Camera*> m_Cameras;
	std::vector<Animator*> m_Animators;

	//========================================================================
	glm::vec2 GetMousePosition() { ConfigureMouse(); return mouse.position; }

	// Tension 2D Functions
	void TensionRendering(Vertex2D* m_Renderer);
	void TensionLayerSort();
	void Vertex2dRendering(Vertex2D* render);

	void ConfigVioletSystems();

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

	// System Related Diagnostics
	int GetDrawCallCount() { return m_CurrentRenderBatchesFromRenderer; }
	void UpdateFramesPerSecond(float _index) { m_CurrentFramesPerSecond = _index; }
	float GetFramesPerSecond() { return m_CurrentFramesPerSecond; }

	// Rendering Systems
	void BeginVertexRenderTextureBindings();
	void CompleteVertexRenderTextureBindings();

	GameObject* GetMainCamera();
private:
	GameObject* GetUserInterfacePicker(glm::vec2 _mouse);
	void BeginColourPickEditor(Vertex2D* m_Renderer);
	void BeginColourPickInterface(Vertex2D* m_Renderer);

	GameObject* GetEditorCamera() { return m_EditorCamera; }
	GameObject* EditorPicker(glm::vec2 _mouse);

	std::unordered_map<uint32_t, GameObject*> m_GameObjectRegister;
	RenderTexture* m_UiRenderTexture;

	GameObject* m_EditorCamera;
	RenderTexture* m_EditorColourPicker;

	int m_CurrentRenderBatchesFromRenderer = 0;
	float m_CurrentFramesPerSecond = 0;

	Camera* m_Vertex_Ui_Camera;
	std::vector<AudioSource*> m_AudioSources;
	ISoundEngine* m_SoundEngine;
	TransparencySorting m_SortingTransparentAlgo;
	Volume m_SceneVolume;
	SceneManager* m_SceneManager;

	std::vector<vGameObject*> m_GameObjects3D;

	PhysicsScene* m_PhysicsScene;
	std::vector<Canvas*> m_CanvasList;
	int m_ActiveCanvasDisplay = 0;

	std::vector<VertexComponent*> m_VertexComponentsList;
	std::vector<GameObject*> m_TransParentList;
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