#include "Cursor.h"

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
