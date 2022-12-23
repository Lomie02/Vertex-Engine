#pragma once
#include "Transform.h"
#include <vector>
#include <string.h>
#include <iostream>
class Clip
{
public:
	Clip();
	Clip(std::string _name) { name = _name; }
	~Clip();

	void AddKeyframe();
	void RemoveKeyframe();
	int Length();
	void PLaySpeed(float _speed);

protected:
	std::vector<Transform*> m_Frames;
	float m_PlaySpeed = 1;
	int m_ClipLength = 0;
	std::string name = "clip";
};

