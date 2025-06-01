#include "PlayerController.h"

// Set everything in the component.
PlayerController::PlayerController()
{
	m_Target = nullptr;
	m_Speed = 2;
}

void PlayerController::AssignPlayer(GameObject* _obj)
{
	m_Target = _obj;
}

void PlayerController::MovePosition(glm::vec2 _direction, float delta)
{
	if (_direction != glm::vec2(0,0)) // Checks if there is any direction input, if false the component will not update position.
	{
		m_Target->transform->position.x += _direction.x * m_Speed * delta;
		m_Target->transform->position.y += _direction.y * m_Speed * delta;
	}
}

void PlayerController::SetSpeed(float _amnount)
{
	m_Speed = _amnount;

	if (m_Speed < 0)
	{
		m_Speed = 0;
	}
}

void PlayerController::Update(float delta)
{
	if (!m_UseGravity && m_Target == nullptr || m_Weight == 0) {
		return;
	}
	m_Target->transform->position.y -= m_Weight * delta;
}

void PlayerController::SetGravity(bool _state)
{
	m_UseGravity = _state;
}

void PlayerController::SetWeight(float _amount)
{
	m_Weight = _amount;
}
