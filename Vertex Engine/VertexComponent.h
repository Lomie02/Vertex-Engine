#pragma once

class vGameObject;
class GameObject;

#include "imgui.h"
class VertexComponent //base class for all Vertex custom scripts & componets.
{
public:
	virtual ~VertexComponent() = default;

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

	vGameObject* partner3d;
	GameObject* partner2d;
};

