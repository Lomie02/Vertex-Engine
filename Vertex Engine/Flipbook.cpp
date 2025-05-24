#include "Flipbook.h"
#include <iostream>
Flipbook::Flipbook()
{
}

void Flipbook::AdjustClipPlaySpeed(std::string _name, float _speed)
{
	for (int i = 0; i < m_AnimationClips.size(); i++) {
		if (m_AnimationClips.at(i).m_Name == _name) {
			m_AnimationClips.at(i).m_PlaySpeed = _speed;
		}
	}
}

bool Flipbook::SetMaster(GameObject* _ref) // sets the master object that will have its material adjusted by the flipbook animation.
{
	switch (m_MasterMode)
	{
	case Singular:
		m_Master = _ref;
		break;
	case Multiple:
		m_MasterFamily.push_back(_ref);
		break;
	}

	return true;
}

bool Flipbook::Play() // plays the animation that is current.
{
	m_IsPlaying = true;
	return true;
}

bool Flipbook::Stop() // stop playing the animation.
{
	m_IsPlaying = false;
	m_CurrentFrame = 0;
	return true;
}

FlipClip Flipbook::AdjustClip(int _index) // returns the clip in the position given as long as its within the vectors size.
{
	if (_index < m_AnimationClips.size()) {
		return m_AnimationClips.at(_index);
	}

	return m_AnimationClips.at(0);
}

FlipClip Flipbook::AdjustClip(std::string _name) // returns the clip with the same name as given.
{
	for (int i = 0; i < m_AnimationClips.size(); i++) {
		if (m_AnimationClips.at(i).m_Name == _name) {
			return m_AnimationClips.at(i);
		}
	}
	return m_AnimationClips.at(0);
}

bool Flipbook::ReplaceClip(int _index, FlipClip _newClip) // Replaced the given clip with a new one given by the user.
{
	if (_index < m_AnimationClips.size()) {
		m_AnimationClips.at(_index) = _newClip;
		return true;
	}
	else {
		return false;
	}
}

bool Flipbook::AddFrame(FlipClip _clip)
{
	m_AnimationClips.push_back(_clip);
	return true;
}

// Delete a animation from the flipbook componenet.
bool Flipbook::RemoveAnimationClip(std::string _name)
{
	for (int i = 0; i < m_AnimationClips.size(); i++)
	{
		if (m_AnimationClips.at(i).m_Name == _name)
		{
			m_AnimationClips.erase(m_AnimationClips.begin() + i);
			return true;
		}
	}
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

void Flipbook::Start()
{
}

void Flipbook::Update(float delta)
{
	if (m_IsPlaying)
	{
		m_Timer += 1 * m_AnimationClips.at(m_ActiveAnimation).m_PlaySpeed * delta;

		if (m_Timer > 2)
		{
			m_Timer = 0;
			//Change the texture to match the current frame.

			if (m_CurrentFrame < m_AnimationClips.at(m_ActiveAnimation).m_Frames.size())
			{
				switch (m_MasterMode) {
				case Singular:

					m_Master->material.AlbedoMap = m_AnimationClips.at(m_ActiveAnimation).m_Frames.at(m_CurrentFrame);
					m_CurrentFrame++;
					break;
				case Multiple:

					for (int i = 0; i < m_MasterFamily.size(); i++) {

						m_MasterFamily.at(i)->material.AlbedoMap = m_AnimationClips.at(m_ActiveAnimation).m_Frames.at(m_CurrentFrame);
					}
					m_CurrentFrame++;
					break;
				}
			}
			else {
				m_CurrentFrame = 0;

				if (!m_AnimationClips.at(m_ActiveAnimation).m_Loop) {
					m_IsPlaying = false;
				}
			}
		}
	}
}

void Flipbook::LateUpdate(float delta)
{
}

void Flipbook::FixedUpdate(float fixedDelta)
{
}
