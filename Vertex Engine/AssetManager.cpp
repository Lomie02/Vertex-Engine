#include "AssetManager.h"
#include <iostream>
#include "ResourceManager.h"
#include <Windows.h>
#include "glad.h"

#include "GameSettings.h"
#include "VertexPrefs.h"

#include "Collider.h"
#include <thread>
#include "GameSettings.h"
#include "RectTransform.h"
#include "SpriteRenderer.h"

/*
	The AssetManager is the engines way of knowing what exists in the game & what to do with the objects.
	This system controls all major things such as:
	- Renderering
	- Updating Objects/ Componenets
	- Collision Detection
	- Physics
	- Sound
	- Layer Sorting
	- Configuring System Updates
*/

AssetManager::~AssetManager() // automatically delete all pointers when asset manager is destroyed. Can be toggled on or off.
{
	if (m_DeleteAssetPointersAuto) {

		for (int i = 0; i < m_Objects.size(); i++) {
			delete m_Objects.at(i);
			m_Objects.at(i) = nullptr;
		}
		for (int i = 0; i < m_Opaque.size(); i++) {
			delete m_Opaque.at(i);
			m_Opaque.at(i) = nullptr;
		}
		for (int i = 0; i < m_Transparent.size(); i++) {
			delete m_Transparent.at(i);
			m_Transparent.at(i) = nullptr;
		}
		for (int i = 0; i < m_UiButtonObjects.size(); i++) {
			delete m_UiButtonObjects.at(i);
			m_UiButtonObjects.at(i) = nullptr;
		}
		for (int i = 0; i < m_AudioSources.size(); i++) {
			delete m_AudioSources.at(i);
			m_AudioSources.at(i) = nullptr;
		}

		delete m_Vertex_Ui_Camera;
		m_Vertex_Ui_Camera = nullptr;

		delete m_PhysicsScene;
		m_PhysicsScene = nullptr;

		delete m_EditorColourPicker;
		m_EditorColourPicker = nullptr;

		if (m_EditorCamera) {
			delete m_EditorCamera;
			m_EditorCamera = nullptr;
		}

		if (m_UiRenderTexture) {
			delete m_UiRenderTexture;
			m_UiRenderTexture = nullptr;
		}

	}
}

/// <summary>
/// Assign the audio engine.
/// </summary>
/// <param name="_engine"></param>
void AssetManager::AssignSoundSystem(irrklang::ISoundEngine* _engine)
{
	m_SoundSystem = _engine;
}

/// <summary>
/// Uses boot settings to keep renderers in check.
/// </summary>
/// <param name="_settings"></param>
void AssetManager::BootUpAll(BootUpContainer* _settings)
{
	if (_settings != nullptr) {
		m_DeleteAssetPointersAuto = _settings->m_AutoDeletePointers;

		if (_settings->m_UseDefaultRenderer == true)
		{
			m_RendererToUse = Vertex_2D;
		}
		else {
			m_RendererToUse = Tension_2D;
		}

		switch (_settings->m_TransparentSortingAlgo)
		{
		case BubbleSort:
			m_SortingTransparentAlgo = BubbleSort;
			std::cout << "VERTEX MESSAGE: Bubble Sort set as transparency layer sorting method." << std::endl;
			break;
		case Quick_Sort:
			std::cout << "VERTEX WARNING: Quicksort is not supported currently, the transparency sorting will revert to default (Bubble Sort)" << std::endl;
			m_SortingTransparentAlgo = BubbleSort;
			break;

		case Insertion_Sort:
			std::cout << "VERTEX MESSAGE: Insertion Sort set as transparency layer sorting method." << std::endl;
			m_SortingTransparentAlgo = Insertion_Sort;
			break;
		}
	}
}

/// <summary>
/// Register a Vertex Component.
/// </summary>
/// <param name="_object"></param>
void AssetManager::Register(VertexComponent* _object)
{
	m_VertexComponentsList.push_back(_object);
}

/// <summary>
/// Register GameObjects.
/// </summary>
/// <param name="_object"></param>
void AssetManager::Register(GameObject* _object)
{
	if (_object) {
		m_Objects.push_back(_object);
	}
	else {
		VERTEX_ERROR("Failed To Register. Object was nullptr.");
	}
}

//TODO: Replace with box2d
void AssetManager::Register(RigidBody* _object)
{
	m_PhysicsScene->Register(_object);
}

/// <summary>
/// Register an Animator
/// </summary>
/// <param name="_object"></param>
void AssetManager::Register(Animator* _object)
{
	m_Animators.push_back(_object);
}

/// <summary>
/// Register world button
/// </summary>
/// <param name="_object"></param>
void AssetManager::Register(Button* _object)
{
	m_UiButtonObjects.push_back(_object);
}

/// <summary>
/// Configure all systems.
/// </summary>
void AssetManager::ConfigureSystems()
{
	if (!m_ShutDownManager) {

		if (m_Cameras.size() > 0)
		{
			ResourceManager::GetShader("sprite").SetMatrix4("pro", m_Cameras.at(m_ActiveCamera)->GetProjection());
			//ResourceManager::GetShader("VertexText").SetMatrix4("projection", m_Cameras.at(m_ActiveCamera)->GetProjection(), true);
		}
		else {
			m_ShutDownManager = true;
			std::cout << "!-VERTEX ERROR: NO ACTIVE CAMERAS IN SCENE-! \n Please register all cameras to a AssetManager." << std::endl;
		}
	}
}

/// <summary>
/// Assigns the renderer to its respected pipeline
/// </summary>
/// <param name="render"></param>
void AssetManager::ConfigureRenderSystems(Vertex2D* render)
{
	switch (m_RendererToUse) {
	case Vertex_2D:
		Vertex2dRendering(render);
		break;

	case Tension_2D:
		TensionRendering(render);
		break;

	case Violet_3D:
		break;
	}
}

//TODO: Remove & replace with box2d
void AssetManager::ConfigurePhysics(float fixedDelta)
{
	m_PhysicsScene->FixedUpdate(fixedDelta);
	UpdateComponents(fixedDelta);
}

/// <summary>
/// Register a Camera.
/// </summary>
/// <param name="camera"></param>
void AssetManager::Register(Camera* camera)
{
	m_Cameras.push_back(camera);
}

/// <summary>
/// Register World Text Objects
/// </summary>
/// <param name="_text"></param>
void AssetManager::Register(Text* _text)
{
	m_UiTextObjects.push_back(_text);
}

/// <summary>
/// Register a Volume (Post Processing Effects)
/// </summary>
/// <param name="_text"></param>
void AssetManager::Register(Volume& _text)
{
	m_SceneVolume = _text;
}

/// <summary>
/// Register NavAgents
/// </summary>
/// <param name="_nav"></param>

/// <summary>
/// Changes the camera used for rendering if more than one exists.
/// </summary>
/// <param name="_index"></param>
void AssetManager::SetActiveCamera(int _index)
{
	m_ActiveCamera = _index;
}

/// <summary>
/// Register Audio Sources
/// </summary>
/// <param name="_audio"></param>
void AssetManager::Register(AudioSource* _audio)
{
	m_AudioSources.push_back(_audio);
}

/// <summary>
/// Register Canvas Interface Objects
/// </summary>
/// <param name="_canvas"></param>
void AssetManager::Register(Canvas* _canvas)
{
	m_CanvasList.push_back(_canvas);
}

void AssetManager::Register(vGameObject* _object)
{
	m_GameObjects3D.push_back(_object);
}

GameObject* AssetManager::RegisterGameObjectNew(GameObject* _parent, GameObject* _child)
{
	GameObject* temp = new GameObject("GameObject");
	temp->material.AlbedoMap = ResourceManager::GetTexture("Girl_01");
	temp->transform->scale.x = 5;

	if (_parent) {
		temp->transform->SetParent(_parent->transform);
	}
	if (_child) {
		temp->transform->SetChild(_child->transform);
	}

	switch (temp->material.surface) {
	case Opaque:
		m_Opaque.push_back(temp);
		break;
	case Transparent:
		m_Transparent.push_back(temp);
		break;
	}

	temp->material.surface = Opaque;
	temp->transform->scale.y = 5;
	Register(temp);

	return temp;
}

//Vertex Tension Renderer 
void AssetManager::TensionRendering(Vertex2D* m_Renderer)
{
	// Render Colour Picking 

	if (m_EditorCamera)
		BeginColourPickEditor(m_Renderer);

	BeginColourPickInterface(m_Renderer);

	// Render Normal Scene ============================
	bool AllowedToRender = false;

	// Create a list that has all the camera components
	std::vector<Camera*> cameraComps;
	for (int i = 0; i < m_Objects.size(); i++) {
		if (m_Objects[i]->GetComponenet<Camera>()) {
			cameraComps.push_back(m_Objects[i]->GetComponenet<Camera>());
		}
	}

	for (Camera* cams : cameraComps) {

		if (cams->GetDisplay() != 0) { //TODO: Change this to allow display 0 cameras to have render textures for Editor use only.

			if (cams->renderTexture) {
				cams->renderTexture->Bind();
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				glViewport(0, 0, cams->renderTexture->GetWidth(), cams->renderTexture->GetHeight());
				AllowedToRender = true;
			}
		}
		else
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glViewport(0, 0, PROJECT_ASPECT_WIDTH, PROJECT_ASPECT_HEIGHT);
			AllowedToRender = true;
		}

		//Bind Editor Camera

		if (AllowedToRender) {


			m_Renderer->StartRenderFrameCycle();
			if (m_SingleSortRenderering && !m_HasRendered) // Sort the transparent layers once only to save performance. Use if objects never change layers in engine.
			{
				TensionLayerSort();
				m_HasRendered = true;
			}
			else if (!m_SingleSortRenderering) // Update the transparency layer sorting every frame. (Exspensive)
			{
				TensionLayerSort();
			}

			m_Renderer->TensionVolume(m_SceneVolume);

			// Render normal sprites.
			for (int i = 0; i < m_Opaque.size(); i++)
			{
				if (m_Opaque.at(i)->GetActive() && m_Opaque.at(i)->GetComponenet<RectTransform>() == nullptr)
				{
					m_Renderer->TensionDraw(m_Opaque.at(i), cams->GetProjection());
				}
			}

			//TODO: Remove this when Rigidbodies become componenets.
			for (int i = 0; i < m_PhysicsScene->Get2dObjects().size(); i++)
			{
				if (m_PhysicsScene->Get2dObjects().at(i)->GetActive())
				{
					m_Renderer->TensionDraw(m_PhysicsScene->Get2dObjects().at(i), cams->GetProjection());
				}
			}

			// Render transparent objects
			for (int i = 0; i < m_Transparent.size(); i++)
			{
				if (m_Transparent.at(i)->GetActive() && m_Transparent.at(i)->GetComponenet<RectTransform>() == nullptr) {

					m_Renderer->TensionDraw(m_Transparent.at(i),cams->GetProjection());

					if (m_Transparent.at(i)->GetMimes().size() > 1)
					{
						for (int m = 0; m < m_Transparent.at(i)->GetActiveMimesSize(); m++) {
							m_Renderer->TensionDraw(m_Transparent.at(i),cams->GetProjection());
						}
					}
				}
			}

			// Render UI Componenets
			// TODO: make ui elements register to a map for faster loading
			for (int i = 0; i < m_Objects.size(); i++)
			{
				if (m_Objects.at(i)->GetActive())
				{
					if (m_Objects.at(i)->GetComponenet<RectTransform>())
						m_Renderer->TensionInterfaceDraw(m_Objects.at(i));

					if (m_Objects.at(i)->GetComponenet<Text>())
						m_Objects.at(i)->GetComponenet<Text>()->ConfigureRenderSystems(glm::ortho(0.0f, static_cast<float>(1920), static_cast<float>(1080), 0.0f, -0.100f, 10.0f));
				}
			}

			if (cams->renderTexture) {
				cams->renderTexture->UnBind();
			}

			AllowedToRender = false;
		}
	}
}

//Tension Renderers Layer Sorting
void AssetManager::TensionLayerSort()
{
	// Seperate Opaque sprites from transparent sprites for tension layer sorting.
	for (int i = 0; i < m_Objects.size(); i++)
	{
		if (m_Objects.at(i)->material.surface == Opaque)
		{
			m_Opaque.push_back(m_Objects.at(i));
		}
		else if (m_Objects.at(i)->material.surface == Transparent)
		{
			m_Transparent.push_back(m_Objects.at(i));
		}
	}

	if (TENSION_TRANSPARENT_LAYER_SORTING) {

		// Local vars are here so the compilier doesnt cry
		std::vector<GameObject*> m_TransparentSortList = m_Transparent;
		int J;
		GameObject* key;
		bool sorted = false;

		switch (m_SortingTransparentAlgo)

		{
		case BubbleSort: // Use the default bubble sort for transparent objects.

			if (m_Transparent.size() > 1) {

				while (!sorted) {
					sorted = true;

					for (int i = 0; i < m_TransparentSortList.size() - 1; i++)
					{
						J = i + 1;
						if (m_TransparentSortList.at(i)->layer >= m_TransparentSortList.at(J)->layer && J < m_TransparentSortList.size())
						{
							key = m_TransparentSortList.at(i);
							m_TransparentSortList.at(i) = m_TransparentSortList.at(J);
							m_TransparentSortList.at(J) = key;
							sorted = false;
						}
					}
				}
				m_Transparent = m_TransparentSortList;

			}
			break;

		case Quick_Sort: // Quicksort method is unsupported at this time.

			QuickSort(m_Transparent, 0, m_Transparent.size() - 1);
			break;
		case Insertion_Sort:

			InsertionSort();
			break;
		}
	}
}

// Vertex Default 2D renderer.
void AssetManager::Vertex2dRendering(Vertex2D* render)
{
	bool IsAllowedToRender = false;
	GameObject* MainCamera = new GameObject();
	for (int i = 0; i < m_Objects.size(); i++) {
		if (m_Objects[i]->GetComponenet<Camera>() && m_Objects[i]->GetComponenet<Camera>()->GetDisplay() == 0 && m_Objects[i]->GetActive()) {
			MainCamera = m_Objects[i];
			IsAllowedToRender = true;
			break;
		}
	}

}

void AssetManager::ConfigVioletSystems()
{
	for (auto* objects : m_GameObjects3D) {
		for (auto& vComp : objects->GetEntireComponenetsList()) {
			vComp->Update(1);
			vComp->FixedUpdate(1);
			vComp->LateUpdate(1);
		}
	}
}

//TODO: Remove this or improve it
void AssetManager::ConfigSetup()
{
}

//TODO: Remove this or improve it
void AssetManager::ExecuteAll()
{
	for (int i = 0; i < m_Objects.size(); i++)
	{
		VertexPrefs::SetInt(m_Objects.at(i)->name, m_Objects.at(i)->layer);
		VertexPrefs::SaveTransform(m_Objects.at(i));
	}

	//VertexPrefs::SaveFile("Scene_data", m_Objects);
}

/// <summary>
/// Unregisters a gameobject so the engine no longer treats it as an object.
/// </summary>
/// <param name="_target"></param>
void AssetManager::UnRegister(GameObject* _target)
{
	if (_target == nullptr) {
		VERTEX_ERROR("Failed to UnRegister GameObject.");
	}
	else
	{
		// Remove from GameObject list
		for (int i = 0; i < m_Objects.size(); i++)
		{
			if (m_Objects.at(i) == _target)
			{
				m_Objects.at(i)->transform->RemoveParent();
				delete m_Objects.at(i);
				m_Objects.at(i) = nullptr;

				m_Objects.erase(m_Objects.begin() + i);

			}
		}

		// Remove from render lists.

		/*bool removedFromRenderList = false;

		for (int i = 0; i < m_Opaque.size(); i++)
		{
			if (m_Opaque.at(i) == _target)
			{
				delete m_Opaque.at(i);
				m_Opaque.at(i) = nullptr;

				m_Opaque.erase(m_Opaque.begin() + i);
				removedFromRenderList = true;
			}
		}

		if (!removedFromRenderList) {

			for (int i = 0; i < m_Transparent.size(); i++)
			{
				if (m_Transparent.at(i) == _target)
				{
					delete m_Transparent.at(i);
					m_Transparent.at(i) = nullptr;

					m_Transparent.erase(m_Transparent.begin() + i);
				}
			}
		}*/
	}
}

// Render all gizmos in editor
void AssetManager::Gizmos(Vertex2D* render)
{

}

GameObject* AssetManager::FindObjectWithTag(std::string _tag)
{

	for (int i = 0; i < m_Objects.size(); i++)
	{
		if (m_Objects.at(i)->Tag == _tag)
		{
			return m_Objects.at(i);
		}
	}

	return nullptr;
}

/// <summary>
/// Finds the object with the mathcing tag & returns it. It goes in order of the objects when they were registered.
/// </summary>
/// <param name="_tag"></param>
/// <returns></returns>
std::vector<GameObject*> AssetManager::FindObjecstWithTag(std::string _tag)
{
	std::vector<GameObject*> TaggedObjects;

	for (int i = 0; i < m_Objects.size(); i++)
	{
		if (m_Objects.at(i)->Tag == _tag)
		{
			TaggedObjects.push_back(m_Objects.at(i));
		}
	}

	return TaggedObjects;
}

/// <summary>
/// Returns an array with all the objects that have the specified componenet
/// </summary>
/// <param name="_ref"></param>
/// <returns></returns>
std::vector<GameObject*> AssetManager::FindObjecstWithComponent(VertexComponent& _ref)
{
	return std::vector<GameObject*>();
}

/// <summary>
/// Finds an object with the matching componenet set & returns it.
/// </summary>
/// <param name="_ref"></param>
/// <returns></returns>

GameObject* AssetManager::FindObjectWithComponent(VertexComponent& _ref)
{

	return nullptr;
}

void AssetManager::BeginVertexRenderTextureBindings()
{
}

void AssetManager::CompleteVertexRenderTextureBindings()
{
}

GameObject* AssetManager::EditorPicker(glm::vec2 _mouse)
{
	glm::u8vec4 pixels = m_EditorColourPicker->ReadPixels(_mouse.x, _mouse.y);

	uint32_t objectId = (pixels.r) | (pixels.g << 8) | (pixels.b << 16);

	if (objectId == 0)
		return nullptr;

	return m_GameObjectRegister[objectId];
}

GameObject* AssetManager::GetMainCamera()
{
	for (int i = 0; i < m_Objects.size(); i++) {
		if (m_Objects[i]->GetComponenet<Camera>() && m_Objects[i]->GetActive()) {
			return m_Objects[i];
			break;
		}
	}
	return nullptr;
}

GameObject* AssetManager::GetUserInterfacePicker(glm::vec2 _mouse)
{
	glm::u8vec4 pixels = m_UiRenderTexture->ReadPixels(_mouse.x, _mouse.y);

	uint32_t objectId = (pixels.r) | (pixels.g << 8) | (pixels.b << 16);

	if (objectId == 0)
		return nullptr;

	return m_GameObjectRegister[objectId];
}

void AssetManager::BeginColourPickEditor(Vertex2D* m_Renderer)
{
	m_EditorCamera->GetComponenet<Camera>()->renderTexture->Bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, m_EditorCamera->GetComponenet<Camera>()->renderTexture->GetWidth(), m_EditorCamera->GetComponenet<Camera>()->renderTexture->GetHeight());

	// Sorting 

	m_Renderer->StartRenderFrameCycle();
	if (m_SingleSortRenderering && !m_HasRendered) // Sort the transparent layers once only to save performance. Use if objects never change layers in engine.
	{
		TensionLayerSort();
		m_HasRendered = true;
	}
	else if (!m_SingleSortRenderering) // Update the transparency layer sorting every frame. (Exspensive)
	{
		TensionLayerSort();
	}

	for (int i = 0; i < m_Opaque.size(); i++)
	{
		if (m_Opaque.at(i)->GetActive())
		{
			m_Renderer->VertexEngineColourPickRender(m_Opaque.at(i),m_EditorCamera->GetComponenet<Camera>()->GetProjection());
		}
	}

	for (int i = 0; i < m_Transparent.size(); i++)
	{
		if (m_Transparent.at(i)->GetActive()) {

			m_Renderer->VertexEngineColourPickRender(m_Transparent.at(i), m_EditorCamera->GetComponenet<Camera>()->GetProjection());

		}
	}


	m_EditorCamera->GetComponenet<Camera>()->renderTexture->UnBind();
}

void AssetManager::BeginColourPickInterface(Vertex2D* m_Renderer)
{
	m_UiRenderTexture->Bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, m_UiRenderTexture->GetWidth(), m_UiRenderTexture->GetHeight());

	for (int i = 0; i < m_Objects.size(); i++)
	{
		if (m_Objects.at(i)->GetActive())
		{
			if (m_Objects.at(i)->GetComponenet<RectTransform>())
				m_Renderer->TensionInterfaceDraw(m_Objects.at(i), true);

		}
	}

	m_UiRenderTexture->UnBind();
}

/// <summary>
/// Updates all components attached to a gameobject.
/// </summary>
/// <param name="delta"></param>

void AssetManager::SwapResources(std::vector<GameObject*> _list, int _element1, int _element2)
{
	GameObject* _temp;
	_temp = _list.at(_element1); // Temp storage for 1st element.

	_list.at(_element1) = _list.at(_element2); // Change element 1 to element 2.

	_list.at(_element2) = _temp;
}

void AssetManager::InsertionSort() //Transparency layer sorting with the insertion sort algo.
{
	int i, j;
	GameObject* key;
	for (i = 1; i < m_Transparent.size(); i++) {
		key = m_Transparent[i];
		j = i - 1;

		while (j >= 0 && m_Transparent[j]->layer >= key->layer) {
			m_Transparent[j + 1] = m_Transparent[j];
			j = j - 1;
		}
		m_Transparent[j + 1] = key;
	}
}

void AssetManager::QuickSort(std::vector<GameObject*> _list, int _start, int _end)
{
	if (_start >= _end)
		return;

	int p = Partition(_list, _start, _end);
	QuickSort(_list, _start, p - 1);
	QuickSort(_list, p + 1, _end);
} // TODO: Fix quicksort

int AssetManager::Partition(std::vector<GameObject*> _list, int _start, int _end)
{
	GameObject* pivot = _list[_start];

	int count = 0;
	for (int i = _start + 1; i <= _end; i++) {
		if (_list[i]->layer <= pivot->layer)
			count++;
	}

	int pivotIndex = _start + count;
	std::swap(_list[pivotIndex], _list[_start]);

	int i = _start, j = _end;

	while (i < pivotIndex && j > pivotIndex) {

		while (_list[i]->layer <= pivot->layer) {
			i++;
		}

		while (_list[j]->layer > pivot->layer) {
			j--;
		}

		if (i < pivotIndex && j > pivotIndex) {
			std::swap(_list[i++], _list[j--]);
		}
	}

	return pivotIndex;
}

void AssetManager::UpdateComponents(float delta) //TODO: Update this system for the new Componenet system
{
	// Update Componenets
	for (auto& comps : m_GameObjects3D) {
		for (auto& data : comps->GetEntireComponenetsList()) {
			data->Update(Time::GetDeltaTime());
			data->FixedUpdate(Time::GetFixedDeltaTime());
			data->LateUpdate(Time::GetDeltaTime());
		}
	}

	for (auto& comps : m_Objects) {
		for (auto& data : comps->GetEntireComponenetList()) {
			data->Update(delta);
			data->FixedUpdate(delta);
			data->LateUpdate(delta);
		}
	}
}

/// <summary>
/// Updates the mouse positon from screen to world space.
/// </summary>

void AssetManager::ConfigureMouse() //TODO: FInd out how to convert the Y cords. X Cords are correct but the Y cords are not.
{
	if (m_Window != nullptr)
	{
		double Xpos;
		double Ypos;

		Transform ConvertedMouseCord;

		glfwGetCursorPos(m_Window, &Xpos, &Ypos);

		if (m_OperatingMode == EDITOR) {
			ConvertedMouseCord.position = glm::unProject(glm::vec3(Xpos, Ypos, 1), glm::mat4(1.0f), m_Cameras.at(m_ActiveCamera)->GetProjection(), glm::vec4(299.973f, 349.968f, 1280, 720));
		}
		else {
			ConvertedMouseCord.position = glm::unProject(glm::vec3(Xpos, Ypos, 1), glm::mat4(1.0f), m_Cameras.at(m_ActiveCamera)->GetProjection(), glm::vec4(0, 0, PROJECT_RESOLUTION));
		}

		//mouse.position.x = ConvertPosition.position.x * m_Cameras.at(m_ActiveCamera)->zoom;
		//mouse.position.y = ConvertPosition.position.y * m_Cameras.at(m_ActiveCamera)->zoom;

		float x = (2.0f * mouse.position.x) / 1920 - 1.0f;
		float y = (2.0f * mouse.position.y) / 1080 - 1.0f;

		mouse.position.x = x * m_Cameras.at(m_ActiveCamera)->zoom;
		mouse.position.y = y * m_Cameras.at(m_ActiveCamera)->zoom;

		std::cout << mouse.position.x << " | " << mouse.position.y << std::endl; //NOTE: This appears to have fixed the Y axes but could be wrong. This is temp for now
	}
}

// Log special events like last objects position & button tracking
void AssetManager::LogEvents()
{

}