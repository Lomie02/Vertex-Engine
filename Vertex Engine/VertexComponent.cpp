#include "VertexComponent.h"

VertexComponent::VertexComponent()
{
	OnEnable();
}

void VertexComponent::OnEnable()
{
	enable = true;
}

void VertexComponent::OnDisable()
{
	enable = false;
}

void VertexComponent::Awake()
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

