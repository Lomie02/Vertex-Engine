#pragma once
#include "glfw3.h"

/*
	Documentation to read https://www.glfw.org/docs/3.3/input_guide.html

	TODO: Implement single press keys.
*/

class Input
{
public:
	//Has Key been pressed
	static bool GetKeyDown(GLFWwindow* window, int key);
	//Has key been released
	static bool GetKeyUp(GLFWwindow* _window, int _index);
	//Is key being held down
	static bool GetKey(GLFWwindow* _window, int _index);

private:
	Input();
};

