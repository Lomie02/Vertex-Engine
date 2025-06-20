#pragma once

class vGameObject;
class GameObject;

#include "yaml-cpp/yaml.h"
#include "imgui.h"
#include "GameUniqueIdentityAsset.h"


class VertexComponent //base class for all Vertex custom scripts & componets.
{
public:
	VertexComponent() {
		m_ID = GameUniqueIdentityAsset::GenerateUniqueIdenityIndex();
		partner2d = nullptr;
		partner3d = nullptr;
	}

	//virtual ~VertexComponent() = default;

	virtual void Awake(); // Called on creation.
	virtual void Start(); // Called at Start of Frame (Once)
	virtual void Update(float delta); // Updates Every Frame

	virtual void FixedUpdate(float delta); //Updated on fixed intervals
	virtual void LateUpdate(float delta); // Called last.

	virtual void RenderEditorDisplay(); // Used for exposing custom componets to the editor.

	// Trigger calls for 2d Pipeline
	virtual void OnTriggerEnter2d();
	virtual void OnTriggerStay2d();
	virtual void OnTriggerExit2d();

	// Trigger calls for 3d Pipeline
	virtual void OnTriggerEnter3d();
	virtual void OnTriggerStay3d();
	virtual void OnTriggerExit3d();

	virtual void OnCollision();

	virtual YAML::Node SerializeComponent() const;
	virtual void DeserializeComponent(const YAML::Node& _node);

	vGameObject* partner3d;

	GameObject* partner2d;

	uint32_t GetUniqueId() { return m_ID; }
protected:
	uint32_t m_ID;

};

