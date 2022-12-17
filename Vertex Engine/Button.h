#pragma once
#include "GameObject.h"
#include <string>
#include "VertexRenderPackage.h"
#include "ResourceManager.h"
class Button : public GameObject
{
public:
	Button() : GameObject()
	{ 
		m_Text = new VertexText2D(1920,1080);
		texture = ResourceManager::GetTexture("UI_Button");
		m_Text->Load("Builds/fonts/arial.ttf", 2);
		UpdateButton();
	}
	Button(const char* Name) : GameObject(Name)
	{ 
		m_Text = new VertexText2D(1920, 1080); 
		texture = ResourceManager::GetTexture("UI_Button");
		m_Text->Load("Builds/fonts/arial.ttf", 2);
		UpdateButton();
	}
	Button(const char* Name, std::string _text) : GameObject(Name)
	{

		m_Text = new VertexText2D(1920, 1080);
		texture = ResourceManager::GetTexture("UI_Button");
		m_Text->Load("Builds/fonts/arial.ttf", 2);
		text = _text;
		UpdateButton();
	}
	Button(const char* Name, bool active) : GameObject(Name, active)
	{ 
		texture = ResourceManager::GetTexture("UI_Button");
		m_Text = new VertexText2D(1920, 1080); 
		m_Text->Load("Builds/fonts/arial.ttf", size);
		UpdateButton();
	}

	~Button();

	bool Pressed() { return m_WasPressed; }
	void UpdateButton();
	std::string text = "Button";
	float size = 2;
	glm::vec3 colour = glm::vec3(1, 0, 0);

private:
	bool m_WasPressed = false;
	VertexText2D* m_Text;
};

