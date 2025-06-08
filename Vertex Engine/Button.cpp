#include "Button.h"

Button::~Button()
{
	delete m_Text;
	m_Text = nullptr;

	delete m_Text;
	m_Text = nullptr;
}

void Button::ConfigureCustoms(glm::mat4 _camera)
{
}

void Button::SetOffset(glm::vec2 _offset)
{
	m_Offset = _offset;
}

void Button::BeginSetUp()
{
	material.AlbedoMap = ResourceManager::GetTexture("UI_Button");
}