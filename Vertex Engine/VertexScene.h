#pragma once
#include "VertexRenderPackage.h"
#include "AssetManager.h"

/*
	Base class that all scenes should follow.
*/

class VertexScene {

public:
	VertexScene() { m_Manager = new AssetManager(); };
	~VertexScene() { delete m_Manager; m_Manager = nullptr; };
	VertexScene(const char _name[]) { strcpy_s(m_SceneName, _name); m_Manager = new AssetManager(); };
	virtual void Awake() = 0;
	virtual void Start() = 0;
	virtual void Update(float delta) = 0;

	virtual void LateUpdate(float delta) = 0;
	virtual void FixedUpdate(float fixedDelta) = 0;

	virtual AssetManager* GetAssets() { return m_Manager; }
	char m_SceneName[15] = "MyScene";

protected:
	AssetManager* m_Manager; // Each scene has a asset manager attached to it. This system allows the engine to know what exists
};