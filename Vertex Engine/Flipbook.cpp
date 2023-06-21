#include "Flipbook.h"

Flipbook::Flipbook()
{
}

bool Flipbook::CreateNewAnimation(std::string _name) //TODO: Figure out a way to create animation clips inside the componenet instead of manually creating them in a scene.
{
	return false;
}

// Find an animation clip that shares the name given & set it as the current playing animation
bool Flipbook::SetAnimation(std::string _name)
{
	for (int i = 0; i < m_AnimationClips.size(); i++)
	{
		if (m_AnimationClips.at(i).m_Name == _name)
		{
			m_ActiveAnimation = i;
			return true;
		}
	}
	return false;
}

// Set the animation playing to be whatever index is given in an array. Must be in range of the arrange size to be considered.
bool Flipbook::SetAnimation(int _index)
{
	if (_index < m_AnimationClips.size() && _index >= 0)
	{
		m_ActiveAnimation = _index;
		return true;
	}
	return false;
}
