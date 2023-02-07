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

	m_Text->transform.position.x = transform.position.x + m_Offset.x;
	m_Text->transform.position.y = transform.position.y - m_Offset.y;
}

void Button::SetOffset(glm::vec2 _offset)
{
	m_Offset = _offset;
}

void Button::BeginSetUp()
{
	m_Text->text = "Play";
}