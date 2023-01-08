#pragma once
#include <iostream>
#include "VertexComponent.h"
class TestComponent : public VertexComponent
{
public:
	TestComponent();
	void Start();
	void Update(float delta);

	std::string name = "Componenet";
private:

};