#include "AssetManager.h"
#include <iostream>
#include "ResourceManager.h"
#include <Windows.h>
#include "glm.hpp"
#include "glad.h"

#include "GameSettings.h"
#include "VertexPrefs.h"

#include "Collider.h"
#include <thread>

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

void AssetManager::AssignSoundSystem(irrklang::ISoundEngine* _engine)
{
	m_SoundSystem = _engine;
}

void AssetManager::BootUpAll(BootUpContainer* _settings)
{
	if (_settings != nullptr) {
		if (_settings->m_UseDefaultRenderer == true)
		{
			m_RendererToUse = Vertex_2D;
		}
		else {
			m_RendererToUse = Tension_2D;
		}
	}
}

void AssetManager::Register(VertexComponent& _object)
{
	m_VertexComponentsList.push_back(_object);
}

void AssetManager::Register(GameObject* _object)
{
	m_Objects.push_back(_object);
	m_PreviousLocations.push_back(&_object->transform);
}

void AssetManager::Register(RigidBody* _object)
{
	m_PhysicsObjects.push_back(_object);
}

void AssetManager::Register(Animator* _object)
{
	m_Animators.push_back(_object);
}

void AssetManager::Register(Button* _object)
{
	m_UiButtonObjects.push_back(_object);
}

void AssetManager::CollisionCheck()
{
	if (m_Objects.size() == 1 || m_Objects.at(0) == nullptr)
	{
		return;
	}

	for (int i = 0; i < m_Objects.size(); i++)
	{
		for (int j = 1; j < m_Objects.size(); j++)
		{
			if (j != i) // Check that the 2 objects are not the same objects.
			{
				Transform Object1;
				Transform Object2;

				Object1 = m_Objects.at(i)->transform;
				Object2 = m_Objects.at(j)->transform;

				if (Object1.position.x < Object2.position.x + Object2.size.x
					&& Object1.position.x + Object1.size.x > Object2.position.x
					&& Object1.position.y < Object2.position.y + Object2.size.y &&
					Object1.position.y + Object1.size.y > Object2.position.y)
				{
					m_Objects.at(i)->transform.position.x = m_PreviousLocations.at(i)->position.x;
					m_Objects.at(i)->transform.position.y = m_PreviousLocations.at(i)->position.y;

					m_Objects.at(j)->transform.position.x = m_PreviousLocations.at(j)->position.x;
					m_Objects.at(j)->transform.position.y = m_PreviousLocations.at(j)->position.y;
				}
			}
		}
	}
}

bool AssetManager::OnTrigger(GameObject* A, GameObject* B)
{
	bool CollisionFound;

	Transform Object1;
	Transform Object2;

	Object1 = A->transform;
	Object2 = B->transform;

	if (Object1.position.x < Object2.position.x + Object2.size.x
		&& Object1.position.x + Object1.size.x > Object2.position.x
		&& Object1.position.y < Object2.position.y + Object2.size.y &&
		Object1.position.y + Object1.size.y > Object2.position.y)
	{
		return true;
	}
	return false;
}

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

		if (m_UiButtonObjects.size() > 0 && m_Window != nullptr)
		{
			if (glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
			{
				for (int i = 0; i < m_UiButtonObjects.size(); i++)
				{
					//============================================================================
					ConfigureMouse();

					Transform button = m_UiButtonObjects.at(i)->transform;
					mouse.size.x = 1;
					mouse.size.y = 1;

					if (button.position.x < mouse.position.x + mouse.size.x
						&& button.position.x + button.size.x > mouse.position.x
						&& button.position.y < mouse.position.y + mouse.size.y &&
						button.position.y + button.size.y > mouse.position.y)
					{
						m_UiButtonObjects.at(i)->PressEvent();
					}

				}

				for (int i = 0; i < m_Objects.size(); i++)
				{
					//============================================================================
					ConfigureMouse();

					Transform ObjectsPosition = m_Objects.at(i)->transform;
					mouse.size.x = 1;
					mouse.size.y = 1;

					if (ObjectsPosition.position.x < mouse.position.x + mouse.size.x
						&& ObjectsPosition.position.x + ObjectsPosition.size.x > mouse.position.x
						&& ObjectsPosition.position.y < mouse.position.y + mouse.size.y &&
						ObjectsPosition.position.y + ObjectsPosition.size.y > mouse.position.y)
					{
						m_Objects.at(i)->transform.position.x = mouse.position.x;
						m_Objects.at(i)->transform.position.y = -mouse.position.y;
					}

				}
			}
		}
	}

	CollisionCheck();
}

void AssetManager::ConfigureRenderSystems(Vertex2D* render)
{
	if (m_RendererToUse == Vertex_2D)
	{
		Vertex2dRendering(render);
	}
	else if (m_RendererToUse == Tension_2D) {
		TensionRendering(render);
	}
}

void AssetManager::ConfigurePhysics(float fixedDelta)
{
	if (m_PhysicsObjects.size() > 0)
	{
		for (int i = 0; i < m_PhysicsObjects.size(); i++)
		{
			if (m_PhysicsObjects.at(i)->m_Active)
			{
				m_PhysicsObjects.at(i)->transform.position += m_PhysicsObjects.at(i)->m_Velocity * fixedDelta;
				m_PhysicsObjects.at(i)->ApplyForce(m_WorldGravity * m_PhysicsObjects.at(i)->GetMass() * fixedDelta);
			}
		}
	}

	UpdateComponents(fixedDelta);
}

void AssetManager::Register(Camera* camera)
{
	m_Cameras.push_back(camera);
}

void AssetManager::Register(Text* _text)
{
	m_UiTextObjects.push_back(_text);
}

/// <summary>
/// Sets the active camera that will be used for the game.
/// </summary>
/// <param name="_index"></param>
void AssetManager::SetActiveCamera(int _index)
{
	m_ActiveCamera = _index;
}

//Improve this to use the new Vertex Collsion System.
bool AssetManager::MousePick(GameObject* _target)
{
	if (m_Objects.size() > 0)
	{
		for (int i = 0; i < m_Objects.size(); i++)
		{
			if (_target->name == m_Objects.at(i)->name)
			{
				//============================================================================
				ConfigureMouse();

				bool colX = m_UiButtonObjects.at(i)->transform.position.x + m_UiButtonObjects.at(i)->transform.size.x >= mouse.position.x
					&& mouse.position.x + 0.2f >= m_UiButtonObjects.at(i)->transform.position.x;

				bool colY = m_UiButtonObjects.at(i)->transform.position.y + m_UiButtonObjects.at(i)->transform.size.y >= mouse.position.y
					&& mouse.position.y + 0.2f >= m_UiButtonObjects.at(i)->transform.position.y;

				//============================================================================

				if (colX && colY)
				{
					return true;
				}
			}
		}
		return false;
	}
	return false;
}

//TODO: Fix/Improve Raycasting
bool AssetManager::Raycast2D(glm::vec2 _pos, glm::vec2 _dir, GameObject& _out, float length)
{
	GameObject* Ray = new GameObject();

	Ray->transform.position = _pos;

	if (_dir.x <= -1) {
		_dir.x = -1;
	}
	else if (_dir.x >= 1) {

		_dir.x = 1;
	}

	if (_dir.y <= -1) {
		_dir.y = -1;
	}
	else if (_dir.y >= 1) {

		_dir.y = 1;
	}

	for (int j = 0; j < length; j++)
	{
		Ray->transform.position += _dir;

		for (int i = 0; i < m_Objects.size(); i++)
		{
			bool colX = m_Objects.at(i)->transform.position.x + m_Objects.at(i)->transform.size.x >= Ray->transform.position.x
				&& Ray->transform.position.x + 0.2f >= m_Objects.at(i)->transform.position.x;

			bool colY = m_Objects.at(i)->transform.position.y + m_Objects.at(i)->transform.size.y >= Ray->transform.position.y
				&& Ray->transform.position.y + 0.2f >= m_Objects.at(i)->transform.position.y;

			if (colX && colY)
			{
				delete Ray;
				Ray = nullptr;

				_out = *m_Objects.at(i);

				return true;
			}
		}
	}

	delete Ray;
	Ray = nullptr;

	return false;
}

//Vertex Tension Renderer 
void AssetManager::TensionRendering(Vertex2D* m_Renderer)
{
	if (m_SingleSortRenderering && !m_HasRendered) // Sort the transparent layers once only to save performance. Use if objects never change layers in engine.
	{
		TensionLayerSort();
		m_HasRendered = true;
	}
	else if (!m_SingleSortRenderering) // Update the transparency layer sorting every frame. (Exspensive)
	{
		TensionLayerSort();
	}

	// Render normal sprites.
	for (int i = 0; i < m_Opaque.size(); i++)
	{
		if (m_Opaque.at(i)->GetActive())
		{
			m_Renderer->TensionDraw(m_Opaque.at(i), m_Opaque.at(i)->material, m_Opaque.at(i)->transform.position,
				m_Opaque.at(i)->transform.size, m_Opaque.at(i)->transform.rotation, m_Opaque.at(i)->transform.scale,
				m_Cameras.at(m_ActiveCamera)->GetProjection(), m_Opaque.at(i)->layer);
		}
	}

	// Render UI Buttons.
	for (int i = 0; i < m_UiButtonObjects.size(); i++)
	{
		if (m_UiButtonObjects.at(i)->GetActive()) {
			m_Renderer->TensionDraw(m_UiButtonObjects.at(i), m_UiButtonObjects.at(i)->material, m_UiButtonObjects.at(i)->transform.position,
				m_UiButtonObjects.at(i)->transform.size, m_UiButtonObjects.at(i)->transform.rotation, m_UiButtonObjects.at(i)->transform.scale,
				m_Cameras.at(m_ActiveCamera)->GetProjection(), m_UiButtonObjects.at(i)->layer);
			m_UiButtonObjects.at(i)->ConfigureCustoms(m_Cameras.at(m_ActiveCamera)->GetProjection());
		}
	}

	//  Render World Text
	for (int i = 0; i < m_UiTextObjects.size(); i++)
	{
		if (m_UiTextObjects.at(i)->GetActive()) {
			m_UiTextObjects.at(i)->ConfigureRenderSystems(m_Cameras.at(m_ActiveCamera)->GetProjection());
		}
	}

	// Render transparent objects
	for (int i = 0; i < m_Transparent.size(); i++)
	{
		if (m_Transparent.at(i)->GetActive()) {

			m_Renderer->TensionDraw(m_Transparent.at(i), m_Transparent.at(i)->material, m_Transparent.at(i)->transform.position,
				m_Transparent.at(i)->transform.size, m_Transparent.at(i)->transform.rotation, m_Transparent.at(i)->transform.scale,
				m_Cameras.at(m_ActiveCamera)->GetProjection(), m_Transparent.at(i)->layer);
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

	//TODO: Implement a better sorting algorthm than bubble sort!
	if (TENSION_TRANSPARENT_LAYER_SORTING) { //Testing with Bubblesort. Bubblesort will NOT stay in the source code & will be converted to insertion sort. Only here for bug testing.

		std::vector<GameObject*> m_TransparentSortList = m_Transparent;

		int J;
		GameObject* key;
		bool sorted = false;
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

		/*GameObject* key;

		for (int i = 0; i < m_TransparentSortList.size() - 1; i++)
		{
			for (int j = i + 1; j < m_TransparentSortList.size(); j++)
			{
				if (m_TransparentSortList.at(j)->layer > m_TransparentSortList.at(i)->layer)
				{
					key = m_TransparentSortList.at(i);
					m_TransparentSortList.at(i) = m_TransparentSortList.at(j);
					m_TransparentSortList.at(j) = key;
				}
			}
		}
		m_Transparent = m_TransparentSortList;*/
	}
}

// Vertex Default 2D renderer.
void AssetManager::Vertex2dRendering(Vertex2D* render)
{
	if (m_Objects.size() > 0)
	{
		for (int i = 0; i < m_Objects.size(); i++)
		{
			float WithinDistance = glm::distance(m_Objects.at(i)->transform.position, m_Cameras.at(m_ActiveCamera)->transform.position);

			if (m_Objects.at(i)->m_Active == true && WithinDistance < CAMERA_DISTANCE_RENDER_LIMIT && m_Objects.at(i)->material.surface == Opaque)
			{
				render->DrawSprite(m_Objects.at(i)->material, m_Objects.at(i)->transform.position, m_Objects.at(i)->transform.size, m_Objects.at(i)->transform.rotation, m_Objects.at(i)->transform.scale, m_Cameras.at(m_ActiveCamera)->GetProjection());
				m_Cameras.at(m_ActiveCamera)->ConfigureSystems();
				m_Objects.at(i)->ConfigureSystems();
			}
		}
	}

	if (m_Objects.size() > 0)
	{
		for (int i = 0; i < m_Objects.size(); i++)
		{
			float WithinDistance = glm::distance(m_Objects.at(i)->transform.position, m_Cameras.at(m_ActiveCamera)->transform.position);

			if (m_Objects.at(i)->m_Active == true && WithinDistance < CAMERA_DISTANCE_RENDER_LIMIT && m_Objects.at(i)->material.surface == Transparent)
			{
				render->DrawSprite(m_Objects.at(i)->material, m_Objects.at(i)->transform.position, m_Objects.at(i)->transform.size, m_Objects.at(i)->transform.rotation, m_Objects.at(i)->transform.scale, m_Cameras.at(m_ActiveCamera)->GetProjection());
				m_Cameras.at(m_ActiveCamera)->ConfigureSystems();
				m_Objects.at(i)->ConfigureSystems();
			}
		}
	}

	if (m_PhysicsObjects.size() > 0)
	{
		for (int i = 0; i < m_PhysicsObjects.size(); i++)
		{
			float WithinDistance = glm::distance(m_PhysicsObjects.at(i)->transform.position, m_Cameras.at(m_ActiveCamera)->transform.position);
			if (m_PhysicsObjects.at(i)->m_Active == true && WithinDistance < CAMERA_DISTANCE_RENDER_LIMIT)
			{
				render->DrawSprite(m_PhysicsObjects.at(i)->material, m_PhysicsObjects.at(i)->transform.position, m_PhysicsObjects.at(i)->transform.size, m_PhysicsObjects.at(i)->transform.rotation, m_PhysicsObjects.at(i)->transform.scale, m_Cameras.at(m_ActiveCamera)->GetProjection());
				m_PhysicsObjects.at(i)->ConfigureSystems();
			}
		}
	}

	if (m_UiObjects.size() > 0)
	{
		for (int i = 0; i < m_UiObjects.size(); i++)
		{
			float WithinDistance = glm::distance(m_UiObjects.at(i)->transform.position, m_Cameras.at(m_ActiveCamera)->transform.position);
			if (m_UiObjects.at(i)->m_Active == true && WithinDistance < CAMERA_DISTANCE_RENDER_LIMIT)
			{
				m_UiObjects.at(i)->ConfigureSystems();
				render->DrawSprite(m_UiObjects.at(i)->material, m_UiObjects.at(i)->transform.position, m_UiObjects.at(i)->transform.size, m_UiObjects.at(i)->transform.rotation, m_UiObjects.at(i)->transform.scale, m_Cameras.at(m_ActiveCamera)->GetProjection());
			}
		}
	}

	//============================================== Render UI Last

	if (m_UiButtonObjects.size() > 0)
	{
		for (int i = 0; i < m_UiButtonObjects.size(); i++)
		{
			float WithinDistance = glm::distance(m_UiButtonObjects.at(i)->transform.position, m_Cameras.at(m_ActiveCamera)->transform.position);
			if (m_UiButtonObjects.at(i)->m_Active == true && WithinDistance < CAMERA_DISTANCE_RENDER_LIMIT)
			{
				m_UiButtonObjects.at(i)->ConfigureSystems();
				render->DrawSprite(m_UiButtonObjects.at(i)->material, m_UiButtonObjects.at(i)->transform.position, m_UiButtonObjects.at(i)->transform.size, m_UiButtonObjects.at(i)->transform.rotation, m_UiButtonObjects.at(i)->transform.scale, m_Cameras.at(m_ActiveCamera)->GetProjection());
				m_UiButtonObjects.at(i)->ConfigureCustoms(m_Cameras.at(m_ActiveCamera)->GetProjection());
			}
		}
	}

	if (m_UiTextObjects.size() > 0)
	{
		for (int i = 0; i < m_UiTextObjects.size(); i++)
		{
			if (m_UiTextObjects.at(i)->m_Active)
			{
				m_UiTextObjects.at(i)->ConfigureRenderSystems(m_Cameras.at(m_ActiveCamera)->GetProjection());
			}
		}
	}
}

//TODO: Remove this or improve it
void AssetManager::ConfigSetup()
{
	for (int i = 0; i < m_Objects.size(); i++)
	{
		m_Objects.at(i)->layer = VertexPrefs::GetInt(m_Objects.at(i)->name);
		m_Objects.at(i)->transform.position = VertexPrefs::GetTransform(m_Objects.at(i)->name).position;
	}

	//VertexPrefs::GetFile("Scene_data", m_Objects);
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
		std::cout << "VERTEX WARNING: " << "Failed to UnRegister GameObject." << std::endl;
	}
	else
	{
		for (int i = 0; i < m_Objects.size(); i++)
		{
			if (m_Objects.at(i) == _target)
			{
				m_Objects.erase(m_Objects.begin() + i);
			}
		}
	}
}

// Render all gizmos in editor
void AssetManager::Gizmos(Vertex2D* render)
{
	if (m_Cameras.size() > 0)
	{
		for (int i = 0; i < m_Cameras.size(); i++)
		{
			if (m_Cameras.at(i)->m_Active == true && i != m_ActiveCamera)
			{
				float HalfSpaceX = m_Cameras.at(i)->transform.size.x / 2;
				float HalfSpaceY = m_Cameras.at(i)->transform.size.y / 2;

				Transform NewPosition;
				NewPosition.position.x = (m_Cameras.at(i)->transform.position.x - HalfSpaceX);
				NewPosition.position.y = (m_Cameras.at(i)->transform.position.y - HalfSpaceY);

				render->DrawSprite(m_CameraGizmo, glm::vec3(NewPosition.position, -5.0f), glm::vec2(136, 98), 0, 0.01f, m_Cameras.at(m_ActiveCamera)->GetProjection());
			}
		}
	}

	render->DrawSprite(m_CenterGizmo, glm::vec2(0, 0), glm::vec2(50, 50), 0, 0.01f, m_Cameras.at(m_ActiveCamera)->GetProjection());

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

/// <summary>
/// Updates all components attached to a gameobject.
/// </summary>
/// <param name="delta"></param>

void AssetManager::UpdateComponents(float delta) //TODO: Update this system for the new Componenet system
{
	for (int i = 0; i < m_VertexComponentsList.size(); i++)
	{
		m_VertexComponentsList.at(i).Update(delta);
		m_VertexComponentsList.at(i).FixedUpdate(delta);
		m_VertexComponentsList.at(i).LateUpdate(delta);
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

		Transform ConvertPosition;

		glfwGetCursorPos(m_Window, &Xpos, &Ypos);

		if (m_OperatingMode == EDITOR) {
			ConvertPosition.position = glm::unProject(glm::vec3(Xpos, Ypos, 1), glm::mat4(1.0f), m_Cameras.at(m_ActiveCamera)->GetProjection(), glm::vec4(299.973f, 349.968f, 1280, 720));
		}
		else {
			ConvertPosition.position = glm::unProject(glm::vec3(Xpos, Ypos, 1), glm::mat4(1.0f), m_Cameras.at(m_ActiveCamera)->GetProjection(), glm::vec4(0, 0, PROJECT_RESOLUTION));
		}

		mouse.position.x = ConvertPosition.position.x * m_Cameras.at(m_ActiveCamera)->zoom;
		mouse.position.y = ConvertPosition.position.y * m_Cameras.at(m_ActiveCamera)->zoom;

		//std::cout << mouse.position.x << " | " << mouse.position.y + 10 << std::endl; //NOTE: This appears to have fixed the Y axes but could be wrong. This is temp for now
	}
}

// Log special events like last objects position & button tracking
void AssetManager::LogEvents()
{
	for (int i = 0; i < m_Objects.size(); i++)
	{
		m_PreviousLocations.at(i)->PreviousPosition.x = m_Objects.at(i)->transform.position.x;
		m_PreviousLocations.at(i)->PreviousPosition.y = m_Objects.at(i)->transform.position.y;
	}

	for (int i = 0; i < m_UiButtonObjects.size(); i++)
	{
		m_UiButtonObjects.at(i)->CloseEvent();
	}
}