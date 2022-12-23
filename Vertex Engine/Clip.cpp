#include "Clip.h"

Clip::Clip()
{
}

Clip::~Clip()
{
}

void Clip::ClearCLip()
{
	m_Frames.clear();
}

void Clip::AddKeyframe(glm::vec2 _frame)
{
	m_Frames.push_back(_frame);
	std::cout << "KeyFrame Added: " << _frame.x << " " << _frame.y << std::endl;
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
