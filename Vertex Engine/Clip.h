#pragma once
#include "Transform.h"
#include <vector>
#include <string.h>
#include <iostream>
#include "glm.hpp"
enum Wrapper
{
	Default = 0,
	Loop,
	RepeatOnce,
};

class Clip
{
public:
	Clip();
	Clip(std::string _name) { name = _name; }
	~Clip();

	void ClearCLip();

	void AddKeyframe(glm::vec2 _frame);
	void RemoveKeyframe(int keyframe, glm::vec2 _frame);
	void PLaySpeed(float _speed);
	float GetPlaySpeed() { return m_PlaySpeed; }
	int Length();

	void SetWrapMode(Wrapper _wrap) { m_WrapMode = _wrap; }
	Wrapper GetWrapMode() { return m_WrapMode; }
	glm::vec2 GetFrame(int _index);
protected:

	Wrapper m_WrapMode;
	std::vector<glm::vec2> m_Frames;
	float m_PlaySpeed = 1;
	int m_ClipLength = 0;
	std::string name = "clip";
};

