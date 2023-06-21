#pragma once
#include "Shader.h"
#include "glm.hpp"
#include "Texture2D.h"
#include <string.h>
#include "ResourceManager.h"

enum Surface {
	Opaque = 0,
	Transparent,
};

class Material
{
public:
	Material();
	Material(Shader& _shader);
	Material(std::string _name);
	Material(std::string _name, Shader& _shader);

	void GetShader(Shader& _shader);

	Shader shader;
	glm::vec4 colour = glm::vec4(1,0,0,1);
	Texture2D baseTexture;

	Surface surface = Opaque;
	bool glow = false;
	std::string name;
	bool m_KeepAspect = false;
	bool m_USeMipMaps = false;
};