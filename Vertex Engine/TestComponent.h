#pragma once
#include "VertexComponent.h"
class TestComponent : public VertexComponent
{
public:
	TestComponent();
	void Start();
	void Update(float delta);
};