#include "AssetManager.h"
#include <iostream>
#include "ResourceManager.h"
#include <Windows.h>
#include "glm.hpp"
#include "glad.h"

#include "GameSettings.h"
#include "VertexPrefs.h"

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

void AssetManager::RegisterUi(GameObject* _object)
{
	m_UiObjects.push_back(_object);
}

void AssetManager::RegisterUi(Button* _object)
{
	m_UiButtonObjects.push_back(_object);
}

bool AssetManager::CollisionCheck()
{
	Collider type1;
	Collider type2;
	if (m_Objects.size() < 1) {
		return false;
	}

	for (int i = 0; i < m_Objects.size(); i++)
	{
		type1 = m_Objects.at(i)->GetCollider();
		for (int j = 0; j < m_Objects.size(); j++)
		{
			type2 = m_Objects.at(j)->GetCollider();
			if (i != j && type1 == AABB && type2 == AABB)
			{
				bool colX = m_Objects.at(i)->transform.position.x + m_Objects.at(i)->transform.size.x >= m_Objects.at(j)->transform.position.x
					&& m_Objects.at(j)->transform.position.x + m_Objects.at(j)->transform.size.x >= m_Objects.at(i)->transform.position.x;

				bool colY = m_Objects.at(i)->transform.position.y - m_Objects.at(i)->transform.size.y >= m_Objects.at(j)->transform.position.y
					&& m_Objects.at(j)->transform.position.y - m_Objects.at(j)->transform.size.y >= m_Objects.at(i)->transform.position.y;

				if (colX && colY)
				{
					m_Objects.at(i)->transform.position = m_PreviousLocations.at(i)->PreviousPosition;
					m_Objects.at(j)->transform.position = m_PreviousLocations.at(j)->PreviousPosition;
					return colX && colY;
				}
			}
		}
	}
	return false;
}

//When the 2 object touch function returns true or false.
bool AssetManager::OnTrigger(GameObject* A, GameObject* B)
{
	Collider type1 = A->GetCollider();
	Collider type2 = B->GetCollider();

	if (type1 != NONE && type1 != NONE) {

		Transform prev = A->transform;
		bool colX = A->transform.position.x + A->transform.size.x >= B->transform.position.x
			&& B->transform.position.x + B->transform.size.x >= A->transform.position.x;

		bool colY = A->transform.position.y - A->transform.size.y >= B->transform.position.y
			&& B->transform.position.y - B->transform.size.y >= A->transform.position.y;

		if (colX && colY)
		{
			return colX && colY;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

void AssetManager::ConfigureSystems()
{
	if (!m_ShutDownManager) {

		if (m_Cameras.size() > 0)
		{
			//ResourceManager::GetShader("sprite").SetMatrix4("pro", m_Cameras.at(m_ActiveCamera)->GetProjection());
			ResourceManager::GetShader("Text").SetMatrix4("projection", m_Cameras.at(m_ActiveCamera)->GetProjection());
		}
		else {
			m_ShutDownManager = true;
			std::cout << "!-VERTEX ERROR: NO ACTIVE CAMERAS IN SCENE-! \n Please register all cameras to a AssetManager." << std::endl;
		}

		if (m_UiButtonObjects.size() > 0)
		{
			if (glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
			{
				for (int i = 0; i < m_UiButtonObjects.size(); i++)
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
						m_UiButtonObjects.at(i)->PressEvent();
					}
				}
			}
		}
	}
}

void AssetManager::ConfigureRenderSystems(Vertex2D* render)
{
	if (m_Objects.size() > 0)
	{
		for (int i = 0; i < m_Objects.size(); i++)
		{
			float WithinDistance = glm::distance(m_Objects.at(i)->transform.position, m_Cameras.at(m_ActiveCamera)->transform.position);

			if (m_Objects.at(i)->m_Active == true && WithinDistance < CAMERA_DISTANCE_RENDER_LIMIT)
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
				render->DrawSprite(m_PhysicsObjects.at(i)->material, m_PhysicsObjects.at(i)->transform.position, m_PhysicsObjects.at(i)->transform.size, m_PhysicsObjects.at(i)->transform.rotation,m_PhysicsObjects.at(i)->transform.scale, m_Cameras.at(m_ActiveCamera)->GetProjection());
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
				render->DrawSprite(m_UiObjects.at(i)->material, m_UiObjects.at(i)->transform.position, m_UiObjects.at(i)->transform.size, m_UiObjects.at(i)->transform.rotation,m_UiObjects.at(i)->transform.scale, m_Cameras.at(m_ActiveCamera)->GetProjection());
			}
		}
	}

	if (m_UiButtonObjects.size() > 0)
	{
		for (int i = 0; i < m_UiButtonObjects.size(); i++)
		{
			float WithinDistance = glm::distance(m_UiButtonObjects.at(i)->transform.position, m_Cameras.at(m_ActiveCamera)->transform.position);
			if (m_UiButtonObjects.at(i)->m_Active == true && WithinDistance < CAMERA_DISTANCE_RENDER_LIMIT)
			{
				m_UiButtonObjects.at(i)->ConfigureSystems();
				render->DrawSprite(m_UiButtonObjects.at(i)->material, m_UiButtonObjects.at(i)->transform.position, m_UiButtonObjects.at(i)->transform.size, m_UiButtonObjects.at(i)->transform.rotation, m_UiButtonObjects.at(i)->transform.scale, m_Cameras.at(m_ActiveCamera)->GetProjection());
				m_UiButtonObjects.at(i)->UpdateButton();
			}
		}
	}

	Material Material;
	Material.colour = glm::vec3(1, 0, 0);

	render->DrawLine(glm::vec2(0, -8), glm::vec2(0, 5), Material);
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
}

void AssetManager::Register(Camera* camera)
{
	m_Cameras.push_back(camera);
}

void AssetManager::SetActiveCamera(int _index)
{
	m_ActiveCamera = _index;
}

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

bool AssetManager::Raycast2D(glm::vec2 _pos, glm::vec2 _dir, GameObject& _out, float length)
{
	GameObject* Ray = new GameObject();

	Ray->transform.position = _pos;

	/*if (_dir.x <= -1) {
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
	}*/

	//std::cout << "Dir: " << _dir.x << " | " << _dir.y << std::endl;

	for (int j = 0; j < length; j++)
	{
		Ray->transform.position += _dir;

		//std::cout << "Raycast Position: " << Ray.transform.position.x << " | " << Ray.transform.position.y << std::endl;

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

void AssetManager::ConfigSetup()
{
	VertexPrefs::GetFile("vertex_scene_data_01.txt", m_Objects);
	VertexPrefs::GetFile("vertex_scene_data_ui_01.txt", m_UiObjects);
}

void AssetManager::ExecuteAll()
{
	VertexPrefs::SaveFile("vertex_scene_data_01.txt", m_Objects);
	VertexPrefs::SaveFile("vertex_scene_data_ui_01.txt", m_UiObjects);
}

void AssetManager::ConfigureMouse()
{
	double Xpos;
	double Ypos;

	glfwGetCursorPos(m_Window, &Xpos, &Ypos);

	mouse.position = glm::unProject(glm::vec3(Xpos, Ypos, 0), glm::mat4(1.0f), m_Cameras.at(m_ActiveCamera)->GetProjection(), glm::vec4(0, 0, PROJECT_RESOLUTION));
}

void AssetManager::LogEvents()
{
	for (int i = 0; i < m_Objects.size(); i++)
	{
		m_PreviousLocations.at(i)->PreviousPosition = m_Objects.at(i)->transform.position;
	}
}

