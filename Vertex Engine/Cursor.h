#pragma once
#include "Graphics.h"
class Cursor
{
public:
	static void Hide(GLFWwindow* _window);
	static void Show(GLFWwindow* _window);

private:
	Cursor();
};

