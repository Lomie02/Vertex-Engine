#include "DebugComp.h"
#include <iostream>

void DebugComp::Start()
{
	std::cout << "Componenet Start" << std::endl;
}

void DebugComp::Update(float delta)
{
	std::cout << "Componenet Updated" << std::endl;
}
