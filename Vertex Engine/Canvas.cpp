#include "Canvas.h"

Canvas::Canvas()
{
}

void Canvas::Add(Sprite* _sprite)
{
	m_SpriteObjects.push_back(_sprite);
}

void Canvas::Add(Button* _button)
{
	m_ButtonObjects.push_back(_button);
}

void Canvas::Add(Text* _text)
{
	m_TextObjects.push_back(_text);
}
