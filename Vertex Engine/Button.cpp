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
}

void Button::BeginSetUp()
{
	m_Text->text = "Play";
	m_Text->transform.localPosition.x += 5;
	m_Text->transform.localPosition.y -= 5;
}