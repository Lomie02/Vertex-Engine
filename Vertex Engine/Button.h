#pragma once
#include "GameObject.h"
#include <string>
#include "VertexRenderPackage.h"
#include "ResourceManager.h"
#include "GameSettings.h"

#include "Text.h"
class Button : public GameObject
{
public:
	Button() : GameObject()
	{ 
		m_Text = new Text();
		m_Text->SetParent(this);

		BeginSetUp();
	}
	Button(const char* Name) : GameObject(Name)
	{ 
		m_Text = new Text();
		m_Text->SetParent(this);

		BeginSetUp();
	}
	Button(const char* Name, std::string _text) : GameObject(Name)
	{
		m_Text = new Text();
		m_Text->SetParent(this);

		BeginSetUp();
	}
	Button(const char* Name, bool active) : GameObject(Name, active)
	{ 
		m_Text = new Text();
		m_Text->SetParent(this);

		BeginSetUp();
	}

	~Button();

	bool Pressed() { return m_WasPressed; }

	void PressEvent() { m_WasPressed = true; };
	void CloseEvent() { m_WasPressed = false; };
	
	void ConfigureCustoms(glm::mat4 _camera);
	
	std::string text = "Button";
	float size = 1.0f;

	void SetOffset(glm::vec2 _offset);

	Text* m_Text;
private:
	glm::vec2 m_Offset = glm::vec2(0, 0);
	void BeginSetUp();
	bool m_WasPressed = false;
};

