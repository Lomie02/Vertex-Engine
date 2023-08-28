#pragma once
#include "Transform.h"

enum CanvasProjectionMode {
	Overlay = 0,
	Universal,
};
class Canvas
{
public:
	void SetActive(bool _state) { m_IsActive = _state; }

	Transform transform;
private:
	CanvasProjectionMode m_ProjectionMode;
	bool m_IsActive = true;
	// a list of UserInterfaceObject types that will replace the GameObject hierachy that the current UI elements follow.
};

