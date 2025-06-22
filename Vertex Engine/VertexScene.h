#pragma once
#include "VertexRenderPackage.h"
#include "AssetManager.h"
#include <string>
#include "GameUniqueIdentityAsset.h"
/*
	Base class that all scenes should follow.
*/

class VertexScene {

public:
	VertexScene() { m_SceneID = GameUniqueIdentityAsset::GenerateUniqueIdenityIndex(); m_Manager = new AssetManager(); };
	~VertexScene() { delete m_Manager; m_Manager = nullptr; };
	VertexScene(std::string _name) { m_SceneName = _name; m_Manager = new AssetManager(); };
	virtual void Awake();
	virtual void Start();
	virtual void Update(float delta);

	virtual void LateUpdate(float delta);
	virtual void FixedUpdate(float fixedDelta);

	virtual AssetManager* GetAssets() { return m_Manager; }
	std::string m_SceneName = "MyScene";
	uint32_t GetUniqueID() { return m_SceneID; }

protected:
	AssetManager* m_Manager; // Each scene has a asset manager attached to it. This system allows the engine to know what exists

private:
	uint32_t m_SceneID;
};