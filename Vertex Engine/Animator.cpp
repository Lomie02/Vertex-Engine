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
	m_Master->transform.position = m_AnimationClip.GetFrame(0);
	std::cout << "Play Animation" << std::endl;
}

void Animator::Pause()
{
	m_IsPlaying = false;
}

void Animator::Stop()
{
	m_IsPlaying = false;
	m_CurrentFrame = 0;
	std::cout << "Stop ANimation" << std::endl;
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
			if (m_CurrentFrame > m_AnimationClip.Length())
			{
				Stop();
			}

			m_CurrentFrame++;
			std::cout << "Next Frame" << std::endl;
		}
	}
}
