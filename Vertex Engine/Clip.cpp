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

void Clip::AddKeyframe(Transform* _frame)
{
	m_Frames.push_back(_frame->position);
	m_FramesSizes.push_back(_frame->scale);
	m_FramesRotations.push_back(_frame->rotation);
}

void Clip::AddKeyframe(Material _frame)
{
	m_FramesColours.push_back(_frame.colour);
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

glm::vec3 Clip::GetFrame(int _index)
{
	return m_Frames.at(_index);
}

glm::vec3 Clip::GetFrameSize(int _index)
{
	return m_FramesSizes.at(_index);
}

glm::vec4 Clip::GetFrameColour(int _index)
{
	return m_FramesColours.at(_index);
}

float Clip::GetFrameRot(int _index)
{
	return 0.0f;
}

void Clip::PLaySpeed(float _speed)
{
	m_PlaySpeed = _speed;
}
