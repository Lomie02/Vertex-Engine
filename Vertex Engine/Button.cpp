#include "Button.h"

Button::~Button()
{
	delete m_Text;
	m_Text = nullptr;
}

void Button::UpdateButton()
{
	m_Text->Text2D(text, transform.position.x, transform.position.y, size, colour);
}