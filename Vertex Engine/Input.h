#pragma once
#include "glfw3.h"
#include <unordered_map>
#include "glm.hpp"

/*
	Vertex Engines new input system. This wraps all of the GLFW into a tidy Input singleton for easy access everywhere.
*/

class Input
{
public:
	// Init the gflw window for input detection. This is already called in the Application.cpp file in the start function.
	static void Init(GLFWwindow* _window) {
		m_GameWindow = _window;
		// Fill all key caches
		std::fill(std::begin(m_CurrentKeys), std::end(m_CurrentKeys), false);
		std::fill(std::begin(m_PreviousFrameKey), std::end(m_PreviousFrameKey), false);
		std::fill(std::begin(m_PreviousGamepadKeys), std::end(m_PreviousGamepadKeys), false);
		std::fill(std::begin(m_CurrentGamepadKey), std::end(m_CurrentGamepadKey), false);

		// Assign each axis map its tag name
		m_LeftAxisStick = "Left Stick";
		m_LeftTrigger = "Left Trigger";
		m_RightAxisStick = "Right Stick";
		m_RightTrigger = "Right Trigger";
	}
	/// <summary>
	/// Returns if true if key was pressed.
	/// </summary>
	/// <param name="key"></param>
	/// <returns></returns>
	static bool GetKeyDown(int key);

	/// <summary>
	/// Returns true if the key is not being pressed
	/// </summary>
	/// <param name="key"></param>
	/// <returns></returns>
	static bool GetKeyUp(int key);

	/// <summary>
	/// Return true if the key is being held down.
	/// </summary>
	/// <param name="key"></param>
	/// <returns></returns>
	static bool GetKey(int key);

	/// <summary>
	/// Updates key inputs. This is only called in the Application before all updates.
	/// </summary>
	static void UpdateInput();

	/// <summary>
	/// Gets the current cursor position in screen space cords.
	/// </summary>
	/// <returns></returns>
	static glm::vec2 GetCursorPos();

	// Check if a gamepad is connected.
	static bool IsGamepad(int key = GLFW_JOYSTICK_1);
	// Gets the name of the connected game pad.
	static std::string GetGamepadName(int key = GLFW_JOYSTICK_1);

	// Returns true if gamepad button is down in current frame.
	static bool GetButtonDown(int key, int _joystick = GLFW_JOYSTICK_1);
	// Returns true if gamepad buttin is up
	static bool GetButtonUp(int key, int _joystick = GLFW_JOYSTICK_1);
	// Returns true if gamepad button is being held down.
	static bool GetButton(int key, int _joystick = GLFW_JOYSTICK_1);

	/// <summary>
	/// Returns a vec2 of gixen axes.
	/// </summary>
	static glm::vec2 GetAxis(std::string _axisName, int joystick = GLFW_JOYSTICK_1);

	// Returns true if give axis has been moved.
	static bool IsAxisMoved(std::string _axisName, float Threshold = 0.2f ,int joystick = GLFW_JOYSTICK_1);

private:
	// Left joystick (L3) name
	static std::string m_LeftAxisStick;
	// Left trigger (LT) name
	static std::string m_LeftTrigger;
	// Right joystick (R3) name
	static std::string m_RightAxisStick;
	// Right trigger (RT) name
	static std::string m_RightTrigger;

	Input();
	// Previously pressed keys
	static bool m_PreviousFrameKey[GLFW_KEY_LAST];
	// Current key pressed.
	static bool  m_CurrentKeys[GLFW_KEY_LAST];

	// Prevous gamepad buttons.
	static bool m_PreviousGamepadKeys[15];
	// Current gamepad buttons.
	static bool m_CurrentGamepadKey[15];

	// Axis map that holds all axis data with a name & a vec2 of each axis.
	static std::unordered_map<std::string, glm::vec2> m_AxisMap;

	// Game window from Application. (Main window)
	static GLFWwindow* m_GameWindow;
};

