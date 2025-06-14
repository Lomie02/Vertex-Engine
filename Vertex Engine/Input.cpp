#include "Input.h"
#include <cmath>
bool Input::m_PreviousFrameKey[GLFW_KEY_LAST];
bool Input::m_CurrentKeys[GLFW_KEY_LAST];
bool Input::m_PreviousGamepadKeys[15];
bool Input::m_CurrentGamepadKey[15];

std::string Input::m_LeftAxisStick;
std::string Input::m_LeftTrigger;
std::string Input::m_RightAxisStick;
std::string Input::m_RightTrigger;

std::unordered_map<std::string, glm::vec2> Input::m_AxisMap;

GLFWwindow* Input::m_GameWindow;

bool Input::GetKeyDown(int key)
{
	return m_CurrentKeys[key] && !m_PreviousFrameKey[key];
}

bool Input::GetKeyUp(int key)
{
	return !m_CurrentKeys[key] && m_PreviousFrameKey[key];
}

bool Input::GetKey(int key)
{
	return m_CurrentKeys[key];
}

void Input::UpdateInput()
{
	for (int i = 0; i < GLFW_KEY_LAST; i++) {
		m_PreviousFrameKey[i] = m_CurrentKeys[i];
		m_CurrentKeys[i] = glfwGetKey(m_GameWindow, i) == GLFW_PRESS;
	}

	// Update controller inputs.
	// TODO: Later add support for mutiple controllers.
	if (IsGamepad()) {
		GLFWgamepadstate state;
		if (glfwGetGamepadState(GLFW_JOYSTICK_1, &state))
		{
			for (int i = 0; i < 15; i++) {
				m_PreviousGamepadKeys[i] = m_CurrentGamepadKey[i];
				m_CurrentGamepadKey[i] = state.buttons[i] == GLFW_PRESS;
			}


		}

	}
}

glm::vec2 Input::GetCursorPos()
{
	static double xpos, ypos;
	glfwGetCursorPos(m_GameWindow, &xpos, &ypos);
	return glm::vec2(xpos, ypos);
}

bool Input::IsGamepad(int key)
{
	return glfwJoystickIsGamepad(key);
}

std::string Input::GetGamepadName(int key)
{
	const char* name = glfwGetGamepadName(key);
	return std::string(name);
}

bool Input::GetButtonDown(int key, int _joystick)
{
	return m_CurrentGamepadKey[key] && !m_PreviousGamepadKeys[key];
}

bool Input::GetButtonUp(int key, int _joystick)
{
	return !m_CurrentGamepadKey[key] && m_PreviousGamepadKeys[key];
}

bool Input::GetButton(int key, int _joystick)
{
	return m_CurrentGamepadKey[key];
}

glm::vec2 Input::GetAxis(std::string _axisName, int joystick)
{
	GLFWgamepadstate state;

	//Gets all values of gamepad axis & stores them inside a map for easy access.
	if (IsGamepad() && glfwGetGamepadState(joystick, &state)) {
		m_AxisMap[m_LeftAxisStick] = glm::vec2(state.axes[GLFW_GAMEPAD_AXIS_LEFT_X], -state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y]); // Left stick
		m_AxisMap[m_LeftTrigger] = glm::vec2(state.axes[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER], 0); // Left Trigger

		m_AxisMap[m_RightAxisStick] = glm::vec2(state.axes[GLFW_GAMEPAD_AXIS_RIGHT_X], -state.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y]); // Right stick
		m_AxisMap[m_RightTrigger] = glm::vec2(state.axes[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER], 0); // Right Trigger
	}

	// Returns a axis map with the given name.
	return m_AxisMap[_axisName];
}

bool Input::IsAxisMoved(std::string _axisName, float Threshold, int joystick)
{
	GLFWgamepadstate state;

	// Update the axis map.
	if (IsGamepad() && glfwGetGamepadState(joystick, &state)) {
		m_AxisMap[m_LeftAxisStick] = glm::vec2(state.axes[GLFW_GAMEPAD_AXIS_LEFT_X], state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y]); // Left stick
		m_AxisMap[m_LeftTrigger] = glm::vec2(state.axes[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER], 0); // Left Trigger

		m_AxisMap[m_RightAxisStick] = glm::vec2(state.axes[GLFW_GAMEPAD_AXIS_RIGHT_X], state.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y]); // Right stick
		m_AxisMap[m_RightTrigger] = glm::vec2(state.axes[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER], 0); // Right Trigger
	}

	// Grab each axis as a float.
	float xAxis = m_AxisMap[_axisName].x;
	float yAxis = m_AxisMap[_axisName].y;

	// If given string is a trigger name, swap the return functions.
	//TODO: Try to clean this up or make it a little easier to look at
	if (_axisName == m_LeftTrigger || _axisName == m_RightTrigger) {

		if (fabs(xAxis) > Threshold || fabs(yAxis) > Threshold) {
			return  false;
		}

		return true;
	}
	else {
		if (fabs(xAxis) > Threshold || fabs(yAxis) > Threshold) {
			return true;
		}

		return  false;
	}
}
