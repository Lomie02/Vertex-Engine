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

void Clip::AddKeyframe(glm::vec2 _frame)
{
	m_Frames.push_back(_frame);
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

void Clip::PLaySpeed(float _speed)
{
	m_PlaySpeed = _speed;
}
