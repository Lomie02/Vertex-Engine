#pragma once
#include "Material.h"
#include "ResourceManager.h"
#include <vector>

class AnimationClip
{
public:
	AnimationClip();
	AnimationClip(std::string _name) : m_ClipName(_name){};

	std::string GetClipName() { return m_ClipName; }
	void AnimationLoopWrap(bool _state) { m_AnimationLooped = _state; }
	void SetClipName(std::string _name) { m_ClipName = _name; }
	void AddFrame(Texture2D _frame) { m_Frames.push_back(_frame); }

	int GetFrameSize() { return m_Frames.size();}
	Texture2D GetFrame(int _index) { return m_Frames.at(_index); }

private:
	std::vector<Texture2D> m_Frames;
	std::string m_ClipName;
	bool m_AnimationLooped = false;
};

