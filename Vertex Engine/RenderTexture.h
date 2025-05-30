#pragma once
#include <iostream>
#include "Graphics.h"
#include "glm.hpp"
class RenderTexture
{
public:
	RenderTexture(unsigned int _width, unsigned int _height);
	~RenderTexture();

	void UpScaleImage(unsigned int _height, unsigned int _width);

	void Bind();
	void UnBind();

	GLuint GetTexture() const { return m_Texture; }
	GLuint GetBuffer() const { return m_Fbo; }

	unsigned int GetWidth() const { return m_Width; }
	unsigned int GetHeight() const { return m_Height; }

	glm::u8vec4 ReadPixels(int _x, int _y) const;

private:
	GLuint m_Fbo = 0;
	GLuint m_DepthBuff = 0;
	GLuint m_Texture = 0;

	unsigned int m_Width;
	unsigned int m_Height;
};

