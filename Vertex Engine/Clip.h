#pragma once
#include "Transform.h"
#include <vector>
#include <string.h>
#include <iostream>
#include "glm.hpp"

#include "Transform.h"
enum Wrapper
{
	Default = 0,
	Loop,
	RepeatOnce,
	PingPong,
};

class Clip
{
public:
	Clip();
	Clip(std::string _name) { name = _name; }
	~Clip();

	void ClearClip();

	void AddKeyframe(Transform _frame);
	void RemoveKeyframe(int keyframe, glm::vec2 _frame);
	void PLaySpeed(float _speed);
	float GetPlaySpeed() { return m_PlaySpeed; }
	int Length();

	void SetWrapMode(Wrapper _wrap) { m_WrapMode = _wrap; }
	Wrapper GetWrapMode() { return m_WrapMode; }
	glm::vec2 GetFrame(int _index);
	glm::vec2 GetFrameSize(int _index);
protected:

	Wrapper m_WrapMode;
	std::vector<glm::vec2> m_Frames;
	std::vector<glm::vec2> m_FramesSizes;
	float m_PlaySpeed = 1;
	int m_ClipLength = 0;
	std::string name = "clip";
};

