#pragma once
#include "Clip.h"
#include "GameObject.h"
#include "gtx/compatibility.hpp"

class Animator
{
public:
	Animator();
	~Animator();

	void SetMaster(GameObject* _master);
	void Play();
	void Pause();
	void Stop();

	void AddKeyFrame();
	void WrapMode(Wrapper _wrap);
	Wrapper GetWrapMode();

	void ConfigureSystems(float deltaTime);

private:

	int m_CurrentFrame = 0;
	bool m_Loop;
	GameObject* m_Master;
	friend class Clip;
	Clip m_AnimationClip;

	bool m_ReadyFrame = true;
	bool m_IsPlaying;

};

