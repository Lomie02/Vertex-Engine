#pragma once
#include "Transform.h"
#include <vector>
#include "Sprite.h"
#include "Button.h"
#include "VertexText2D.h"

class Canvas {
public:
	Canvas();
	Canvas(const char* _name) { m_CanvasName = _name; }

	bool IsActive() { return m_IsActiveCanvas; }
	void SetActive(bool _state) { m_IsActiveCanvas = _state; }

	void Add(Sprite* _sprite);
	void Add(Button* _button);
	void Add(Text* _text);

	std::vector<Sprite*> GetSprites() { return m_SpriteObjects; }
	std::vector<Button*> GetButtons() { return m_ButtonObjects; }
	std::vector<Text*> GetText() { return m_TextObjects; }

private:
	const char* m_CanvasName = "Canvas";
	bool m_IsActiveCanvas = true;

	std::vector<Sprite*> m_SpriteObjects;
	std::vector<Button*> m_ButtonObjects;
	std::vector<Text*> m_TextObjects;
};