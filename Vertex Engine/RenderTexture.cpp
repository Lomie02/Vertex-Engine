#include "RenderTexture.h"
#include <algorithm>
RenderTexture::RenderTexture(unsigned int _width, unsigned int _height)
{
	m_Height = _height;
	m_Width = _width;

	glGenTextures(1, &m_Texture);
	glBindTexture(GL_TEXTURE_2D, m_Texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glGenFramebuffers(1, &m_Fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_Fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Texture, 0);

	glGenRenderbuffers(1, &m_DepthBuff);
	glBindRenderbuffer(GL_RENDERBUFFER, m_DepthBuff);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_Width, m_Height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_DepthBuff);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "VERTEX ERROR: RENDER TEXTURE IS NOT COMPLETE IN FRAMEBUFFER." << std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

RenderTexture::~RenderTexture()
{
	glDeleteFramebuffers(1, &m_Fbo);
	glDeleteTextures(1, &m_Texture);
	glDeleteRenderbuffers(1, &m_DepthBuff);
}

void RenderTexture::UpScaleImage(unsigned int _height, unsigned int _width)
{
	m_Height = _height;
	m_Width = _width;

	glDeleteTextures(1, &m_Texture);
	glDeleteRenderbuffers(1, &m_DepthBuff);

	glGenTextures(1, &m_Texture);
	glBindTexture(GL_TEXTURE_2D, m_Texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenFramebuffers(1, &m_Fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_Fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Texture, 0);

	glGenRenderbuffers(1, &m_DepthBuff);
	glBindRenderbuffer(GL_RENDERBUFFER, m_DepthBuff);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_Width, m_Height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_DepthBuff);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderTexture::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_Fbo);
	glViewport(0, 0, m_Width, m_Height);
}

void RenderTexture::UnBind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

glm::u8vec4 RenderTexture::ReadPixels(int _x, int _y) const
{
	glm::u8vec4 pixel(0);

	_x = std::clamp(_x, 0, (int)GetWidth() - 1);
	_y = std::clamp(_y, 0, (int)GetHeight() - 1);

	glBindFramebuffer(GL_FRAMEBUFFER, m_Fbo);
	glReadBuffer(GL_COLOR_ATTACHMENT0);
	glReadPixels(_x,_y, 1,1, GL_RGBA, GL_UNSIGNED_BYTE, &pixel);
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return pixel;
}
