#include "AssetManager.h"
#include <iostream>
#include "ResourceManager.h"
#include <Windows.h>

void AssetManager::Register(GameObject* _object)
{
	m_Objects.push_back(_object);
	m_PreviousLocations.push_back(&_object->transform);
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

	if (!m_ShutDownManager && type1 != NONE && type1 != NONE) {

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
					
					double Xpos;
					double Ypos;

					glfwGetCursorPos(m_Window, &Xpos, &Ypos);

					float ConPosX = (Xpos / 2.0f);
					float ConPosY = (Ypos / 2.0f);
					
					std::cout << "X: " << ConPosX / 2 << std::endl;
					std::cout << "Y: " << ConPosY / 2 << std::endl;

					mouse.position.x = ConPosX;
					mouse.position.y = ConPosY;

					bool colX = m_UiButtonObjects.at(i)->transform.position.x + m_UiButtonObjects.at(i)->transform.size.x <= mouse.position.x
						&& mouse.position.x >= m_UiButtonObjects.at(i)->transform.position.x + m_UiButtonObjects.at(i)->transform.size.x;

					bool colY = m_UiButtonObjects.at(i)->transform.position.y - m_UiButtonObjects.at(i)->transform.size.y >= mouse.position.y
						&& mouse.position.y >= m_UiButtonObjects.at(i)->transform.position.y - m_UiButtonObjects.at(i)->transform.size.y;

					if (colX && colY)
					{
						std::cout << "Pressed Button" << std::endl;
						//m_UiButtonObjects.at(i)->Pressed();
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
			if (m_Objects.at(i)->m_Active == true)
			{
				render->DrawSprite(m_Objects.at(i)->texture, m_Objects.at(i)->transform.position, m_Objects.at(i)->transform.size, m_Objects.at(i)->transform.rotation, m_Cameras.at(m_ActiveCamera)->GetProjection());
				m_Cameras.at(m_ActiveCamera)->ConfigureSystems();
				m_Objects.at(i)->ConfigureSystems();
			}
		}
	}

	if (m_UiObjects.size() > 0)
	{
		for (int i = 0; i < m_UiObjects.size(); i++)
		{
			if (m_UiObjects.at(i)->m_Active == true)
			{
				m_UiObjects.at(i)->ConfigureSystems();
				render->DrawSprite(m_UiObjects.at(i)->texture, m_UiObjects.at(i)->transform.position, m_UiObjects.at(i)->transform.size, m_UiObjects.at(i)->transform.rotation, m_Cameras.at(m_ActiveCamera)->GetProjection());
			}
		}
	}

	if (m_UiButtonObjects.size() > 0)
	{
		for (int i = 0; i < m_UiButtonObjects.size(); i++)
		{
			if (m_UiButtonObjects.at(i)->m_Active == true)
			{
				m_UiButtonObjects.at(i)->ConfigureSystems();
				render->DrawSprite(m_UiButtonObjects.at(i)->texture, m_UiButtonObjects.at(i)->transform.position, m_UiButtonObjects.at(i)->transform.size, m_UiButtonObjects.at(i)->transform.rotation, m_Cameras.at(m_ActiveCamera)->GetProjection());
				m_UiButtonObjects.at(i)->UpdateButton();
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

void AssetManager::LogEvents()
{
	for (int i = 0; i < m_Objects.size(); i++)
	{
		m_PreviousLocations.at(i)->PreviousPosition = m_Objects.at(i)->transform.position;
	}
}
