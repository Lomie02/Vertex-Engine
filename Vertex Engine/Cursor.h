#pragma once
#include "Graphics.h"

enum CursorLockMode {
	unlocked = 0,
	Locked,
};

/// <summary>
/// Singleton that controls the Cursor.
/// </summary>
/// 
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
	/// <summary>
	/// Sets if the cursor should lock to the center of the screen. This will also hide the cursor.
	/// </summary>
	static void SetMouseState(GLFWwindow* _window, CursorLockMode _lockmode);

private:
	Cursor();
};