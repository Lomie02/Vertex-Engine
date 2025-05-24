#pragma once
#include "Shader.h"
#include "glm.hpp"
#include "Texture2D.h"
#include <string.h>
#include "ResourceManager.h"

#include "Material.h"
#include "VertexComponent.h"
enum Surface {
	Opaque = 0,
	Transparent,
};

class Material : public VertexComponent
{
public:
	Material();
	Material(Shader& _shader);
	Material(std::string _name);
	Material(std::string _name, Shader& _shader);

	void SetShader(Shader& _shader);

	Shader shader;
	
	// texture maps
	Texture2D AlbedoMap;
	Texture2D MetallicMap;
	Texture2D RoughnessMap;
	Texture2D NormalMap;


	glm::vec4 colour = glm::vec4(1,0,0,1);
	TransparentMode TransparencyBlend = Alpha;
	Surface surface = Opaque;
	bool glow = false;
	
	std::string name;
	bool m_KeepAspect = false;
	bool m_USeMipMaps = false;
};