#pragma once
#include "VertexComponent.h"
#include "Material.h"
#include "ResourceManager.h"
#include "Texture2D.h"

#include <vector>
struct Animation 
{
public:
	std::string m_Name = "Animation";
	std::vector<Texture2D> m_Frames;
	float m_PlaySpeed = 0;
	bool m_Loop = false;
};

class Flipbook : VertexComponent
{
public:
	Flipbook();
	bool CreateNewAnimation(std::string _name);

	bool SetAnimation(std::string _name);
	bool SetAnimation(int _index);

private:
	std::vector<Animation> m_AnimationClips;
	int m_ActiveAnimation = 0;
};

