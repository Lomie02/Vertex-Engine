#pragma once
#include "GameObject.h"
#include <string>
#include "VertexComponent.h"
#include "VertexEvent.h"

#include "SpriteRenderer.h"
class Button : public VertexComponent
{
	friend class AssetManager; // Only let the asset manager of the scene deal with input.
public:
	// Default Constructor
	Button();
	// De-Constructor
	~Button();

	void Awake() override;

	/// <summary>
	/// Callback for when the button is clicked on.
	/// </summary>
	VertexEvent* onClick;

	/// <summary>
	/// Editor Display
	/// </summary>
	void RenderEditorDisplay() override;

	/// <summary>
	/// Updates buttons colours
	/// </summary>
	/// <param name="deltaTime"></param>
	void Update(float deltaTime) override;


private:

	// Triggers when mouse is over 
	void OnHoverEnter();
	// Triggers when mouse exits
	void OnHoverExit();
	// triggered when clicked.
	void OnClicked();
	// Triggered when selected.
	void OnSelected();

	// Can the button be clicked
	bool m_IsInteractable = true;

	// Wrapper to make vec drag floats cleaner with imgui
	void ImGuiVec4(glm::vec4 _colourVec);

	glm::vec4 m_NormalColour; // Default Colour
	glm::vec4 m_SelectedColour; // Selected Colour
	glm::vec4 m_PressedColour; // Pressed Colour
	glm::vec4 m_HoverColour; // Hover Colour
	glm::vec4 m_DisabledColour; //Disabled Colour
	
	// Sprite renderer linked to the main gameobject.
	SpriteRenderer* m_SpriteRendererComp;
};

