#include "VertexComponent.h"

VertexComponent::VertexComponent()
{
}

void VertexComponent::Start()
{
}

void VertexComponent::Update(float delta)
{
}

void VertexComponent::FixedUpdate(float delta)
{
}

void VertexComponent::LateUpdate(float delta)
{
}

void VertexComponent::AssignPartner(GameObject* _partner)
{
	Partner = _partner;
}

void VertexComponent::RemovePartner()
{
	Partner = nullptr;
}
