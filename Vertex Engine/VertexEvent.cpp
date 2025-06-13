#include "VertexEvent.h"

VertexEvent::VertexEvent()
{
}

void VertexEvent::AddListener(std::function<void()> _callback)
{
	m_CallbackEvents.push_back(_callback);
}

void VertexEvent::Invoke()
{
	for (auto& callbacks : m_CallbackEvents) {
		callbacks();
	}
}

void VertexEvent::RemoveListeners()
{
	m_CallbackEvents.clear();
}
