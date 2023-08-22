#pragma once
#include "VertexComponent.h"
#include "Material.h"
#include "ResourceManager.h"
#include "Texture2D.h"
#include "GameObject.h"

#include <vector>
struct FlipClip 
{
public:
	std::string m_Name = "Animation";
	std::vector<Texture2D> m_Frames;
	float m_PlaySpeed = 1;
	bool m_Loop = false;
};

enum FlipBookMasterMode {
	Singular = 0,
	Multiple,
};

class Flipbook : public VertexComponent
{
public:
	Flipbook();

	void SetMasterMode(FlipBookMasterMode _mode) { m_MasterMode = _mode; }
	void AdjustClipPlaySpeed(std::string _name, float _speed);

	bool SetMaster(GameObject* _ref);
	bool Play();
	bool Stop();

	FlipClip AdjustClip(int _index);
	FlipClip AdjustClip(std::string _name);

	bool ReplaceClip(int _index, FlipClip _newClip);
	bool AddFrame(FlipClip _clip);
	bool RemoveAnimationClip(std::string _name);

	bool SetAnimation(std::string _name);
	bool SetAnimation(int _index);
	void Start();
	void Update(float delta);
	void LateUpdate(float delta);
	void FixedUpdate(float fixedDelta);

private:

	GameObject* m_Master;
	bool m_IsPlaying = false;
	std::vector<FlipClip> m_AnimationClips;
	std::vector<GameObject*> m_MasterFamily;
	int m_ActiveAnimation = 0;

	FlipBookMasterMode m_MasterMode = Singular;

	float m_CurrentFrame = 0;
	float m_Timer = 0;
};

