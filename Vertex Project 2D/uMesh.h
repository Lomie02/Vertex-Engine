#pragma once
#include "glm.hpp"
#include <iostream>
#include <vector>
#include "Shader.h"
struct uVertex
{
	glm::vec3 u_Position;
	glm::vec3 u_Normal;
	glm::vec3 u_TexCoords;
};

struct uTexture {
	unsigned int u_Id;
	std::string u_Type;
};


class uMesh
{
public:
	uMesh(std::vector<uVertex> vertices, std::vector<unsigned int> indices, std::vector<uTexture> textures);
	std::vector<uVertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<uTexture> textures;

	void RenderMesh(Shader& _shader);
private:

	unsigned int VAO, VBO, EBO;
	void BeginMeshBirth();
};

