#include "Cursor.h"
/*
*	GLFW INPUT DOCUMENTATION FOR FUTURE REF
	https://www.glfw.org/docs/3.1/input.html#input_cursor_mode
*/

void Cursor::Hide(GLFWwindow* _window)
{
	if (_window != nullptr) {
		glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
}

void Cursor::Show(GLFWwindow* _window)
{
	if (_window != nullptr) {
		glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

void Cursor::SetMouseState(GLFWwindow* _window, CursorLockMode _lockmode)
{
	switch (_lockmode) {
	case unlocked:
		glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		break;
	case Locked:
		glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		break;
	}
}
