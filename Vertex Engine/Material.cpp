#include "Material.h"

Material::Material()
{
	name = "Material";
	shader = ResourceManager::GetShader("sprite");
	colour = glm::vec4(1, 1, 1,1);
}

Material::Material(Shader& _shader)
{
	name = "Material";
	shader = _shader;
	colour = glm::vec4(1, 1, 1,1);
}

Material::Material(std::string _name, Shader& _shader)
{
	name = _name;
	shader = _shader;
	colour = glm::vec4(1, 1, 1,1);
}

void Material::SetShader(Shader& _shader)
{
	shader = shader;
}

Material::Material(std::string _name)
{
	name = _name;
	shader = ResourceManager::GetShader("sprite");
	colour = glm::vec4(1, 1, 1, 1);
}