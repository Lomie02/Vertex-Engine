#pragma once
#include "GameObject.h"
#include <string>
#include "VertexRenderPackage.h"
#include "ResourceManager.h"
#include "GameSettings.h"
class Button : public GameObject
{
public:
	Button() : GameObject()
	{ 
		m_Text = new VertexText2D(PROJECT_RESOLUTION);
		material.baseTexture = ResourceManager::GetTexture("UI_Button");
		m_Text->Load("Builds/fonts/arial.ttf", 24);
		UpdateButton();
	}
	Button(const char* Name) : GameObject(Name)
	{ 
		m_Text = new VertexText2D(PROJECT_RESOLUTION); 
		material.baseTexture = ResourceManager::GetTexture("UI_Button");
		m_Text->Load("Builds/fonts/arial.ttf", 24);
		UpdateButton();
	}
	Button(const char* Name, std::string _text) : GameObject(Name)
	{

		m_Text = new VertexText2D(PROJECT_RESOLUTION);
		material.baseTexture = ResourceManager::GetTexture("UI_Button");
		m_Text->Load("Builds/fonts/arial.ttf", 24);
		text = _text;
		UpdateButton();
	}
	Button(const char* Name, bool active) : GameObject(Name, active)
	{ 
		material.baseTexture = ResourceManager::GetTexture("UI_Button");
		m_Text = new VertexText2D(PROJECT_RESOLUTION);
		m_Text->Load("Builds/fonts/arial.ttf", 24);
		UpdateButton();
	}

	~Button();

	bool Pressed() { return m_WasPressed; }
	void UpdateButton();

	void PressEvent() { m_WasPressed = true; };
	void CloseEvent() { m_WasPressed = false; };
	std::string text = "Button";
	float size = 1.0f;

	VertexText2D* GetVertexText() { return m_Text; }

private:
	bool m_WasPressed = false;
	VertexText2D* m_Text;
};

