#pragma once
#include "GameObject.h"
#include "VertexComponent.h"
class PlayerController : public VertexComponent
{
public:
	PlayerController();

	// Assign the player to the script
	void AssignPlayer(GameObject* _obj);

	//Moves the target in the specified direction.
	void MovePosition(glm::vec2 _direction, float delta);

	// Sets the speed of the target.
	void SetSpeed(float _amnount);

	// Get the speed the player is moving at.
	float GetSpeed() { return m_Speed; }

	// Updates the gravity for the componenet. Function called in the asset manager when components get updated.
	void Update(float delta);

	// Set if the component should apply gravity to the target.
	void SetGravity(bool _state);

	// Set the weight of the object.
	void SetWeight(float _amount);

private:
	GameObject* m_Target; // Player pointer
	float m_Speed = 2;

	float m_Weight = 1;
	bool m_UseGravity = false;
};

