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
	m_Text->ConfigureRenderSystems(_camera);
	m_Text->text = text;

	m_Text->transform->position.x = transform->GetCenter().x + m_Offset.x;
	m_Text->transform->position.y = transform->GetCenter().y + m_Offset.y;
}

void Button::SetOffset(glm::vec2 _offset)
{
	m_Offset = _offset;
}

void Button::BeginSetUp()
{
	m_Text->text = "Button";
	material.AlbedoMap = ResourceManager::GetTexture("UI_Button");
}