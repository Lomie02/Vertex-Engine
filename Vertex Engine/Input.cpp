#include "Input.h"

bool Input::GetKeyDown(GLFWwindow* window, int key)
{
	if (window == nullptr)
		return false;

	if (glfwGetKey(window, key) == GLFW_PRESS)
	{
		return true;
	}

	return false;
}

bool Input::GetKeyUp(GLFWwindow* _window, int _index)
{
	if (_window == nullptr)
		return false;

	if (glfwGetKey(_window, _index) == GLFW_RELEASE)
	{
		return true;
	}
	return false;
}

bool Input::GetKey(GLFWwindow* _window, int _index)
{
	if (_window == nullptr)
		return false;

	if (glfwGetKey(_window, _index) == GLFW_REPEAT)
	{
		return true;
	}

	return false;
}

void Input::SetKeyCallBack(GLFWwindow* _window, GLFWkeyfun _func)
{
	glfwSetKeyCallback(_window, _func);
}

