#include "Animator.h"

Animator::Animator()
{
	m_IsPlaying = false;
}

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

void Animator::AddKeyFrame()
{
	m_AnimationClip.AddKeyframe(m_Master->transform.position);
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
	if (m_IsPlaying)
	{
		if (m_Master->transform.position != m_AnimationClip.GetFrame(m_CurrentFrame))
		{
			m_Master->transform.position = glm::lerp(m_Master->transform.position, m_AnimationClip.GetFrame(m_CurrentFrame), m_AnimationClip.GetPlaySpeed() * deltaTime);
		}
		else
		{
			if (m_CurrentFrame <= m_AnimationClip.Length() && m_WrapMode == Default)
			{
				m_CurrentFrame++;
			}
			else {
				m_IsPlaying = false;
				m_CurrentFrame = 0;
			}

			if (m_CurrentFrame <= m_AnimationClip.Length() && m_WrapMode == Loop)
			{
				m_CurrentFrame++;
			}
			else {
				m_CurrentFrame = 0;
			}
		}
	}
}
