#pragma once
#include "Clip.h"
#include "GameObject.h"
#include "gtx/compatibility.hpp"
#include <vector>
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

	void SetClip(int _index);
	void AddClip(int _location, Clip& _clip);
	void SwapClips(Clip& target, Clip& other); //TODO: Implement clip swapping method

	void ConfigureSystems(float deltaTime);

private:

	bool m_Reverse = false;
	int m_CurrentFrame = 0;
	bool m_Loop;
	GameObject* m_Master;
	friend class Clip;
	Clip m_DefaultClip;

	int m_CurrentClip = 0;
	std::vector<Clip> m_AnimationClip;

	bool m_ReadyFrame = true;
	bool m_IsPlaying;

};

