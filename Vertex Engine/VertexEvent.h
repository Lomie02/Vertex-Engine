#pragma once
#include <vector>
#include <functional>

/*
	Vertex Engines custom event system.
*/
class VertexEvent
{
public:
	VertexEvent();

	/// <summary>
	/// Add a callback.
	/// </summary>
	/// <param name="_callback"></param>
	void AddListener(std::function<void()> _callback);

	/// <summary>
	/// Calls all listeners.
	/// </summary>
	void Invoke();

	/// <summary>
	/// Removes all listners from this event.
	/// </summary>
	void RemoveListeners();

private:
	// List of callbacks.
	std::vector<std::function<void()>> m_CallbackEvents;
};

