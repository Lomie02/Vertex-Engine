#pragma once
#include "Graphics.h"

/// <summary>
/// Singleton that controls the Cursor.
/// </summary>
class Cursor
{
public:
	/// <summary>
	/// Hides the cursor.
	/// </summary>
	/// <param name="Needs a pointer to the GLFW window."></param>
	static void Hide(GLFWwindow* _window);
	/// <summary>
	/// Shows the cursor.
	/// </summary>
	/// <param name="Needs a pointer to the GLFW window."></param>
	static void Show(GLFWwindow* _window);

private:
	Cursor();
};