#include "Animator.h"

Animator::~Animator()
{
}

void Animator::SetMaster(GameObject* _master)
{
	m_Master = _master;
}

void Animator::Play()
{
	m_IsPlaying = true;
}

void Animator::Pause()
{
	m_IsPlaying = false;
}

void Animator::Stop()
{
	m_IsPlaying = false;
}

void Animator::WrapMode(Wrapper _wrap)
{
	m_WrapMode = _wrap;
}

Wrapper Animator::GetWrapMode()
{
	return m_WrapMode;
}

void Animator::ConfigureSystems(float deltaTime)
{
	if (m_IsPlaying && m_CurrentFrame <= m_AnimationClip.Length())
	{
		if (m_Master->transform.position != m_AnimationClip.GetFrame(m_CurrentFrame))
		{
			m_Master->transform.position = glm::lerp(m_Master->transform.position, m_AnimationClip.GetFrame(m_CurrentFrame), m_AnimationClip.GetPlaySpeed() * deltaTime);
		}
		else
		{
			m_CurrentFrame++;
		}
	}
	else 
	{
		if (m_IsPlaying)
		{
			m_IsPlaying = false;
			m_CurrentFrame = 0;
		}
	}
}
