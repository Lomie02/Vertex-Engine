#include "Clip.h"

Clip::Clip()
{
	m_WrapMode = Default;
}

Clip::~Clip()
{
	ClearClip();
}

void Clip::ClearClip()
{
	m_Frames.clear();
}

void Clip::AddKeyframe(Transform _frame)
{
	m_Frames.push_back(_frame.position);
	m_FramesSizes.push_back(_frame.size);
}

void Clip::RemoveKeyframe(int keyframe, glm::vec2 _frame)
{
	//TODO: add remove clips
}

int Clip::Length()
{
	m_ClipLength = m_Frames.size();

	return m_ClipLength;
}

glm::vec2 Clip::GetFrame(int _index)
{
	return m_Frames.at(_index);
}

glm::vec2 Clip::GetFrameSize(int _index)
{
	return m_FramesSizes.at(_index);
}

void Clip::PLaySpeed(float _speed)
{
	m_PlaySpeed = _speed;
}
