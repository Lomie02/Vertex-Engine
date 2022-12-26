#include "Animator.h"

Animator::Animator()
{
	m_IsPlaying = false;
	m_DefaultClip.SetWrapMode(PingPong);
	m_AnimationClip.push_back(m_DefaultClip);
}

Animator::~Animator()
{
	m_AnimationClip.clear();
}

void Animator::SetMaster(GameObject* _master)
{
	m_Master = _master;
}

void Animator::Play()
{
	if (!m_IsPlaying) {
		m_IsPlaying = true;
		m_Master->transform.position = m_AnimationClip.at(m_CurrentClip).GetFrame(0);
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
		m_Master->transform.position = m_AnimationClip.at(m_CurrentClip).GetFrame(0);
		std::cout << "Stop ANimation" << std::endl;
	}
}

void Animator::AddKeyFrame()
{
	m_AnimationClip.at(m_CurrentClip).AddKeyframe(m_Master->transform.position);
}

void Animator::WrapMode(Wrapper _wrap)
{
	m_AnimationClip.at(m_CurrentClip).SetWrapMode(_wrap);
}

Wrapper Animator::GetWrapMode()
{
	return m_AnimationClip.at(m_CurrentClip).GetWrapMode();
}

void Animator::SetClip(int _index)
{
	if (_index < m_AnimationClip.size())
	{
		m_CurrentClip = _index;
	}
}

void Animator::AddClip(int _location, Clip& _clip)
{
	m_AnimationClip.at(_location) = _clip;
}

void Animator::SwapClips(Clip& target, Clip& other)
{
}

void Animator::ConfigureSystems(float deltaTime)
{
	if (m_IsPlaying)
	{
		if (!m_ReadyFrame && m_IsPlaying)
		{
			if (!m_Reverse)
			{
				m_CurrentFrame++;
			}
			else {

				m_CurrentFrame--;
			}

			m_ReadyFrame = true;

			if (m_CurrentFrame >= m_AnimationClip.at(m_CurrentClip).Length() && m_AnimationClip.at(m_CurrentClip).GetWrapMode() == Default)
			{
				Stop();
			}
			else if (m_CurrentFrame >= m_AnimationClip.at(m_CurrentClip).Length() && m_AnimationClip.at(m_CurrentClip).GetWrapMode() == Loop)
			{
				m_CurrentFrame = 0;
				if (m_Master->GetParent() == nullptr)
				{
					m_Master->transform.position = m_AnimationClip.at(m_CurrentClip).GetFrame(0);
				}
				else
				{
					m_Master->transform.localPosition = m_AnimationClip.at(m_CurrentClip).GetFrame(0);
				}
			}
			else if (m_CurrentFrame >= m_AnimationClip.at(m_CurrentClip).Length() && m_AnimationClip.at(m_CurrentClip).GetWrapMode() == PingPong)
			{
				m_Reverse = true;
				m_CurrentFrame = m_AnimationClip.at(m_CurrentClip).Length() - 1;

				if (m_Master->GetParent() == nullptr)
				{
					m_Master->transform.position = m_AnimationClip.at(m_CurrentClip).GetFrame(m_AnimationClip.at(m_CurrentClip).Length() - 1);
				}
				else
				{
					m_Master->transform.localPosition = m_AnimationClip.at(m_CurrentClip).GetFrame(m_AnimationClip.at(m_CurrentClip).Length() - 1);
				}
			}

		}
		else
		{
			if (m_CurrentFrame < m_AnimationClip.at(m_CurrentClip).Length() && !m_Reverse)
			{
				if (m_Master->GetParent() == nullptr)
				{
					m_Master->transform.position = glm::lerp(m_Master->transform.position, m_AnimationClip.at(m_CurrentClip).GetFrame(m_CurrentFrame), m_AnimationClip.at(m_CurrentClip).GetPlaySpeed() * deltaTime);

					float distanceToFrame = glm::distance(m_Master->transform.position, m_AnimationClip.at(m_CurrentClip).GetFrame(m_CurrentFrame));

					if (distanceToFrame < 0.5f) {
						m_ReadyFrame = false;
					}
				}
				else {
					m_Master->transform.localPosition = glm::lerp(m_Master->transform.localPosition, m_AnimationClip.at(m_CurrentClip).GetFrame(m_CurrentFrame), m_AnimationClip.at(m_CurrentClip).GetPlaySpeed() * deltaTime);
					float distanceToFrame = glm::distance(m_Master->transform.localPosition, m_AnimationClip.at(m_CurrentClip).GetFrame(m_CurrentFrame));

					if (distanceToFrame < 0.5f) {
						m_ReadyFrame = false;
					}
				}
			}
			if (m_Reverse && m_CurrentFrame >= 0)
			{
				if (m_Master->GetParent() == nullptr)
				{
					m_Master->transform.position = glm::lerp(m_Master->transform.position, m_AnimationClip.at(m_CurrentClip).GetFrame(m_CurrentFrame), m_AnimationClip.at(m_CurrentClip).GetPlaySpeed() * deltaTime);
					float distanceToFrame = glm::distance(m_Master->transform.position, m_AnimationClip.at(m_CurrentClip).GetFrame(m_CurrentFrame));

					if (distanceToFrame < 0.5f) {
						m_ReadyFrame = false;
					}
				}
				else {
					m_Master->transform.localPosition = glm::lerp(m_Master->transform.localPosition, m_AnimationClip.at(m_CurrentClip).GetFrame(m_CurrentFrame), m_AnimationClip.at(m_CurrentClip).GetPlaySpeed() * deltaTime);
					float distanceToFrame = glm::distance(m_Master->transform.localPosition, m_AnimationClip.at(m_CurrentClip).GetFrame(m_CurrentFrame));

					if (distanceToFrame < 0.5f) {
						m_ReadyFrame = false;
					}
				}
			}
			else
			{
				Stop();
			}
		}
	}
}
