#include "vTexture3D.h"

#include "stb_image.h"

vTexture3D::vTexture3D(std::string _name)
{
	unsigned char* TextureData = stbi_load(_name.c_str(), &m_TextureWidth, &m_TextureHeight, &m_CompressionSize, 0);
	m_HasTextureFinishedLoading = true;

	glGenTextures(1, &m_TextureImageID);
	BindVioletShader();

	if (m_CompressionSize == 3) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_TextureWidth, m_TextureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else if (m_CompressionSize == 4)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_TextureWidth, m_TextureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, TextureData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		m_HasTextureFinishedLoading = false;
		glDeleteTextures(1, &m_TextureImageID);
	}

	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	stbi_image_free(TextureData);
}

vTexture3D::~vTexture3D()
{
	if (m_TextureImageID) {
		glDeleteTextures(1, &m_TextureImageID);
	}
}

void vTexture3D::BindVioletShader(int texture)
{
	glActiveTexture(GL_TEXTURE0 + texture);
	glBindTexture(GL_TEXTURE_2D, m_TextureImageID);
}

void vTexture3D::UnBindVioletShader(int texture)
{
	glBindTexture(GL_TEXTURE_2D, 0 + texture);
}
