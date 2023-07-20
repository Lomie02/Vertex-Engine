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
	bool CreateNewAnimation(Animation _clip);
	bool RemoveAnimationClip(std::string _name);

	bool SetAnimation(std::string _name);
	bool SetAnimation(int _index);
	void Update(float delta);

private:
	std::vector<Animation> m_AnimationClips;
	int m_ActiveAnimation = 0;
};

