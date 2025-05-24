#pragma once
#include "VertexComponent.h"

class DebugComp : public VertexComponent
{
public:
	DebugComp() { Start(); };

	void Start() override;
	 void Update(float delta) override;

};

