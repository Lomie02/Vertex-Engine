#pragma once
#include "Shader.h"
#include "glm.hpp"
#include "Texture2D.h"
#include <string.h>
#include "ResourceManager.h"

class Material
{
public:
	Material();
	Material(Shader& _shader);
	Material(std::string _name);
	Material(std::string _name, Shader& _shader);

	void GetShader(Shader& _shader);

	Shader shader;
	glm::vec3 colour;
	Texture2D baseTexture;

	std::string name;
};