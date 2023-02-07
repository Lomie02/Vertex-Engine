#include "AssetManager.h"
#include <iostream>
#include "ResourceManager.h"
#include <Windows.h>
#include "glm.hpp"
#include "glad.h"

#include "GameSettings.h"
#include "VertexPrefs.h"

#include "Collider.h"

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

bool AssetManager::CollisionCheck()
{


	/*Collider type1;
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
					&& m_Objects.at(j)->transform.position.x + m_Objects.at(j)->transform.size.x >= m_UiButtonObjects.at(i)->transform.position.x;

				bool colY = m_Objects.at(i)->transform.position.y + m_Objects.at(i)->transform.size.y >= m_Objects.at(j)->transform.position.y
					&& m_Objects.at(j)->transform.position.y + m_Objects.at(j)->transform.size.y >= m_Objects.at(i)->transform.position.y;

				if (colX && colY)
				{
					m_Objects.at(i)->transform.position = m_PreviousLocations.at(i)->PreviousPosition;
					m_Objects.at(j)->transform.position = m_PreviousLocations.at(j)->PreviousPosition;
					return colX && colY;
				}
			}
		}
	}
	return false;*/

	for (int k = 0; k < m_Objects.size(); k++) {

		Collider* c1 = m_Objects.at(k)->GetCollider();

		for (int j = 1; j < m_Objects.size(); j++) {

			Collider* c2 = m_Objects.at(j)->GetCollider();

			if (k != j) {

				if (m_Objects.at(k)->GetCollider()->GetType() == Convex && m_Objects.at(j)->GetCollider()->GetType() == Convex) {

					bool ret = true;

					int c1_faces = c1->m_Vertices.size();
					int c2_faces = c2->m_Vertices.size();


					for (int i = 0; i < c1_faces; i++)
					{
						c1->m_Vertices.at(i).y = m_Objects.at(i)->transform.position.y;
						float fx = c1->m_Vertices[i].x - c1->m_Vertices[(i + 1) % c1_faces].x;
						float fy = c1->m_Vertices[i].y - c1->m_Vertices[(i + 1) % c1_faces].y;


						float ax = -fy, ay = fx;

						float len_v = sqrt(ax * ax + ay * ay);
						ax /= len_v;
						ay /= len_v;

						float c1_min = FLT_MAX, c1_max = -FLT_MAX;
						float c2_min = FLT_MAX, c2_max = -FLT_MAX;

						for (int j = 0; j < c1_faces; j++)
						{
							float c1_proj = (ax * (c1->m_Vertices[j].x + c1->tX) + ay * (c1->m_Vertices[j].y + c1->tY)) / (ax * ax + ay * ay);
							c1_min = min(c1_proj, c1_min);
							c1_max = max(c1_proj, c1_max);
						}

						for (int j = 0; j < c2_faces; j++)
						{
							float c2_proj = (ax * (c2->m_Vertices[j].x + c2->tX) + ay * (c2->m_Vertices[j].y + c2->tY)) / (ax * ax + ay * ay);
							c2_min = min(c2_proj, c2_min);
							c2_max = max(c2_proj, c2_max);
						}

						if (!(c1_max >= c2_min && c1_min <= c2_max))
							ret = false;

						if (ret == true)
						{
							m_Objects.at(k)->transform.position = m_PreviousLocations.at(k)->position;
							m_Objects.at(j)->transform.position = m_PreviousLocations.at(j)->position;
						}

						return ret;
					}
				}
				else if (m_Objects.at(k)->GetCollider()->GetType() == AABB && m_Objects.at(j)->GetCollider()->GetType() == AABB)
				{
					bool colX = m_Objects.at(k)->transform.position.x + m_Objects.at(k)->transform.size.x >= m_Objects.at(j)->transform.position.x
						&& m_Objects.at(j)->transform.position.x + m_Objects.at(j)->transform.size.x >= m_UiButtonObjects.at(k)->transform.position.x;

					bool colY = m_Objects.at(k)->transform.position.y + m_Objects.at(k)->transform.size.y >= m_Objects.at(j)->transform.position.y
						&& m_Objects.at(j)->transform.position.y + m_Objects.at(j)->transform.size.y >= m_Objects.at(k)->transform.position.y;

					if (colX && colY)
					{
						m_Objects.at(k)->transform.position = m_PreviousLocations.at(k)->PreviousPosition;
						m_Objects.at(j)->transform.position = m_PreviousLocations.at(j)->PreviousPosition;
						return colX && colY;
					}
				}
				else if (m_Objects.at(k)->GetCollider()->GetType() == Circle && m_Objects.at(k)->GetCollider()->GetType() == Circle)
				{
					float distance = glm::distance(m_Objects.at(k)->transform.position, m_Objects.at(j)->transform.position);
					float radius = m_Objects.at(k)->GetCollider()->ColliderSizeCircle() + m_Objects.at(j)->GetCollider()->ColliderSizeCircle();
					if (distance < radius)
					{
						m_Objects.at(k)->transform.position = m_PreviousLocations.at(k)->position;
						m_Objects.at(j)->transform.position = m_PreviousLocations.at(j)->position;
					}
				}
				else if (m_Objects.at(k)->GetCollider()->GetType() == AABB && m_Objects.at(j)->GetCollider()->GetType() == Circle) {

					//TODO: Add AABB to Circle
				}
				else
				{
					return false;
				}
			}
		}
	}
	return false;
}

bool AssetManager::OnTrigger(GameObject* A, GameObject* B)
{
	Collider* type1 = A->GetCollider();
	Collider* type2 = B->GetCollider();

	if (type1->GetType() == AABB && type1->GetType() == AABB) {

		Transform prev = A->transform;
		bool colX = A->transform.position.x + A->transform.size.x >= B->transform.position.x
			&& B->transform.position.x + B->transform.size.x >= A->transform.position.x;

		bool colY = A->transform.position.y - A->transform.size.y <= B->transform.position.y
			&& B->transform.position.y - B->transform.size.y <= A->transform.position.y;

		if (colX && colY)
		{
			return colX && colY;
		}
		return false;
	}
	else if (type1->GetType() == Circle && type1->GetType() == Circle)
	{
		float distance = glm::distance(A->transform.position, B->transform.position);
		float radius = type1->ColliderSizeCircle() + type2->ColliderSizeCircle();
		if (distance < radius)
		{
			return true;
		}

		return false;
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

		if (m_UiButtonObjects.size() > 0)
		{
			if (glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
			{
				for (int i = 0; i < m_UiButtonObjects.size(); i++)
				{
					//============================================================================
					ConfigureMouse();

					bool colX = m_UiButtonObjects.at(i)->transform.position.x + m_UiButtonObjects.at(i)->transform.size.x >= mouse.position.x
						&& mouse.position.x + 0.5f >= m_UiButtonObjects.at(i)->transform.position.x;

					bool colY = m_UiButtonObjects.at(i)->transform.position.y - m_UiButtonObjects.at(i)->transform.size.y >= mouse.position.y
						&& mouse.position.y - 0.5f <= m_UiButtonObjects.at(i)->transform.position.y;

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

void AssetManager::TensionRendering(Vertex2D* m_Renderer)
{
	if (m_SingleSortRenderering && !m_HasRendered)
	{
		TensionLayerSort();
		m_HasRendered = true;
	}
	else if (!m_SingleSortRenderering)
	{
		TensionLayerSort();
	}

	for (int i = 0; i < m_Opaque.size(); i++)
	{
		m_Renderer->TensionDraw(m_Opaque.at(i), m_Opaque.at(i)->material, m_Opaque.at(i)->transform.position,
			m_Opaque.at(i)->transform.size, m_Opaque.at(i)->transform.rotation, m_Opaque.at(i)->transform.scale,
			m_Cameras.at(m_ActiveCamera)->GetProjection(), m_Opaque.at(i)->layer);
	}

	for (int i = 0; i < m_Transparent.size(); i++)
	{
		m_Renderer->TensionDraw(m_Transparent.at(i), m_Transparent.at(i)->material, m_Transparent.at(i)->transform.position,
			m_Transparent.at(i)->transform.size, m_Transparent.at(i)->transform.rotation, m_Transparent.at(i)->transform.scale,
			m_Cameras.at(m_ActiveCamera)->GetProjection(), m_Transparent.at(i)->layer);
	}

	for (int i = 0; i < m_UiButtonObjects.size(); i++)
	{
		m_Renderer->TensionDraw(m_UiButtonObjects.at(i), m_UiButtonObjects.at(i)->material, m_UiButtonObjects.at(i)->transform.position,
			m_UiButtonObjects.at(i)->transform.size, m_UiButtonObjects.at(i)->transform.rotation, m_UiButtonObjects.at(i)->transform.scale,
			m_Cameras.at(m_ActiveCamera)->GetProjection(), m_UiButtonObjects.at(i)->layer);

		m_UiButtonObjects.at(i)->ConfigureCustoms(m_Cameras.at(m_ActiveCamera)->GetProjection());
	}
}

void AssetManager::TensionLayerSort()
{

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
}

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

void AssetManager::ConfigSetup()
{
	for (int i = 0; i < m_Objects.size(); i++)
	{
		m_Objects.at(i)->layer = VertexPrefs::GetInt(m_Objects.at(i)->name);
		m_Objects.at(i)->transform.position = VertexPrefs::GetTransform(m_Objects.at(i)->name).position;
	}

	//VertexPrefs::GetFile("Scene_data", m_Objects);
}

void AssetManager::ExecuteAll()
{
	for (int i = 0; i < m_Objects.size(); i++)
	{
		VertexPrefs::SetInt(m_Objects.at(i)->name, m_Objects.at(i)->layer);
		VertexPrefs::SaveTransform(m_Objects.at(i));
	}

	//VertexPrefs::SaveFile("Scene_data", m_Objects);
}

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

std::vector<GameObject*> AssetManager::FindObjecstWithComponent(VertexComponent& _ref)
{
	return std::vector<GameObject*>();
}

GameObject* AssetManager::FindObjectWithComponent(VertexComponent& _ref)
{
	for (int i = 0; i < m_Objects.size(); i++)
	{
		std::vector<VertexComponent*> comps = m_Objects.at(i)->ComponentList();
		for (int k = 0; k < comps.size(); k++) {

			if (typeid(_ref) == typeid(comps.at(k)))
			{
				return m_Objects.at(i);
			}
		}
	}

	return nullptr;
}

void AssetManager::UpdateComponents(float delta)
{
	for (int i = 0; i < m_Objects.size(); i++)
	{
		if (m_Objects.at(i)->ComponentCount() > 0)
		{
			std::vector<VertexComponent*> components;
			components = m_Objects.at(i)->ComponentList();

			for (auto comp : components)
			{
				comp->Update(delta);
				comp->FixedUpdate(delta);
				comp->LateUpdate(delta);
			}
		}
	}
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

