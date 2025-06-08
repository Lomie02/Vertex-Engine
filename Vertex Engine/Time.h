#pragma once
/* 
	Vertex Engines new time singleton class allows for global access for DeltaTime, system will no longer need delta to be passed through update functions.
	The Engine will still support delta passed update function so both can be used. However new systems will no longer use the legacy update(delta) method.
*/

class Time
{
	// Allow only the Application class to update delta
	friend class Application;
public:
	/// <summary>
	/// Get Current Time Scale
	/// </summary>
	/// <returns></returns>
	static float GetTimeScale();
	
	/// <summary>
	/// Set Time Scale
	/// </summary>
	/// <param name="_scale"></param>
	static void SetTimeScale(float _scale);

	/// <summary>
	/// Get Delta Time
	/// </summary>
	/// <returns></returns>
	static float GetDeltaTime();
	/// <summary>
	/// Get Fixed Delta, use for physics.
	/// </summary>
	/// <returns></returns>
	static float GetFixedDeltaTime();
	/// <summary>
	/// Get Time Scale in an unscaled state. Unscaled delta time is not affected by time scale.
	/// </summary>
	/// <returns></returns>
	static float GetUnscaledDeltaTime();
	/// <summary>
	/// Set Physics Time Step
	/// </summary>
	/// <param name="_step"></param>
	static void SetTimeStep(float _step = 0.01f);
	// Get Time Step
	static float GetTimeStep();
	/// <summary>
	/// Get FPS
	/// </summary>
	/// <returns></returns>
	static float GetFramesPerSecond();

protected:// Only for Application.cpp

	/// <summary>
	/// Prepare For Calculations
	/// </summary>
	static void PrepareDeltaCalculations();
	/// <summary>
	/// Reset Fixed Delta
	/// </summary>
	static void ResetFixedDelta();
	/// <summary>
	/// Updates Delta Time Calculations Every Frame.
	/// </summary>
	static void ConfigureDeltaTime();


private:
	Time();
	static float m_FixedDelta;

	static float m_ApplicationFramesPerSecond;
	static float m_frames;
	static float m_UnScaledDelta;
	static float m_DeltaTime;

	static float m_fpsInterval;
	static float m_prevTime;
	static float m_TimeStep;
	static float m_TimeScale;

};

