#pragma once
#include "Clip.h"
#include "GameObject.h"

enum Wrapper
{
	Default = 0,
	Loop,
	RepeatOnce,
};

class Animator
{
	Animator();
	~Animator();

	void SetMaster(GameObject* _master);
	void Play();
	void Pause();
	void Stop();

	void WrapMode(Wrapper _wrap);
	Wrapper GetWrapMode();


private:
	Wrapper m_WrapMode;
	bool m_Loop;
	GameObject* m_Master;
	friend class Clip;
	Clip m_AnimationClip;

};

