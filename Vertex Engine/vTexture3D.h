#pragma once
#include <string>
#include "Graphics.h"
class vTexture3D
{
public:
	vTexture3D(std::string _name);
	~vTexture3D();

	vTexture3D(const vTexture3D& other) = delete;
	vTexture3D& operator=(const vTexture3D& other) = delete;

	void BindVioletShader(int texture = 0);
	static void UnBindVioletShader(int texture = 0);

	GLuint m_TextureImageID;
	bool m_HasTextureFinishedLoading;
	int m_TextureWidth;
	int m_TextureHeight;
	int m_CompressionSize;

};

