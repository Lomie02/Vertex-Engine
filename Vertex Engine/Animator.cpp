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
	if (!m_IsPlaying) {
		m_IsPlaying = true;
		m_Master->transform.position = m_AnimationClip.GetFrame(0);
		std::cout << "Play Animation" << std::endl;
	}
}

void Animator::Pause()
{
	m_IsPlaying = false;
}

void Animator::Stop()
{
	if (m_IsPlaying)
	{
		m_IsPlaying = false;
		m_CurrentFrame = 0;
		m_Master->transform.position = m_AnimationClip.GetFrame(0);
		std::cout << "Stop ANimation" << std::endl;
	}
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
		if (!m_ReadyFrame && m_IsPlaying)
		{
			m_CurrentFrame++;
			m_ReadyFrame = true;

			if (m_CurrentFrame >= m_AnimationClip.Length())
			{
				Stop();
			}

			std::cout << "CURRENT FRAME: " << m_CurrentFrame << " Length:  " << m_AnimationClip.Length() << std::endl;
			std::cout << "Next Frame" << std::endl;
		}
		else
		{
			if (m_CurrentFrame < m_AnimationClip.Length())
			{
				m_Master->transform.position = glm::lerp(m_Master->transform.position, m_AnimationClip.GetFrame(m_CurrentFrame), m_AnimationClip.GetPlaySpeed() * deltaTime);

				float distanceToFrame = glm::distance(m_Master->transform.position, m_AnimationClip.GetFrame(m_CurrentFrame));

				if (distanceToFrame < 0.5f) {
					m_ReadyFrame = false;
				}
			}
		}
	}
}
