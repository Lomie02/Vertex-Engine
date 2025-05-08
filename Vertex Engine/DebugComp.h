#pragma once
#include "VertexComponent.h"

class DebugComp : public VertexComponent
{
public:
	DebugComp() {};
	 void Update(float delta) override;

};

