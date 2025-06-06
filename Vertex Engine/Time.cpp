#include "Time.h"
#include <glfw3.h>

float Time::m_FixedDelta = 0;
float Time::m_ApplicationFramesPerSecond = 0;
float Time::m_frames = 0;

float Time::m_UnScaledDelta = 0;
float Time::m_DeltaTime = 0;
float Time::m_fpsInterval = 0;

float Time::m_prevTime = 0;
float Time::m_TimeStep = 0.01f;
float Time::m_TimeScale = 1;

float Time::GetTimeScale()
{
	return m_TimeScale;
}

void Time::SetTimeScale(float _scale)
{
	m_TimeScale = _scale;
}

float Time::GetDeltaTime()
{
	return m_DeltaTime;
}

float Time::GetFixedDeltaTime()
{
	return m_FixedDelta;
}

float Time::GetUnscaledDeltaTime()
{
	return m_UnScaledDelta;
}

void Time::SetTimeStep(float _step)
{
	m_TimeStep = _step;
}

float Time::GetTimeStep()
{
	return m_TimeStep;
}

float Time::GetFramesPerSecond()
{
	return m_ApplicationFramesPerSecond;
}

void Time::PrepareDeltaCalculations()
{
	m_ApplicationFramesPerSecond = 0;
	m_frames = 0;
	m_UnScaledDelta = 0;
	m_fpsInterval = 0;
	m_prevTime = (float)glfwGetTime();
}

void Time::ResetFixedDelta()
{
	m_FixedDelta -= m_TimeStep;
}

void Time::ConfigureDeltaTime()
{
	double currTime = (float)glfwGetTime();
	m_UnScaledDelta = currTime - m_prevTime;
	m_prevTime = currTime;

	// Calulate delta time
	m_frames++;
	m_fpsInterval += m_UnScaledDelta;

	if (m_fpsInterval >= 1.0f)
	{
		m_ApplicationFramesPerSecond = m_frames;
		m_frames = 0;
		m_fpsInterval -= 1.0f;
	}

	m_FixedDelta += m_UnScaledDelta * m_TimeScale;
	m_DeltaTime = m_UnScaledDelta * m_TimeScale;
}
