#include "uMesh.h"
#include "Graphics.h"

uMesh::uMesh(std::vector<uVertex> vertices, std::vector<unsigned int> indices, std::vector<uTexture> textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	BeginMeshBirth();
}

void uMesh::BeginMeshBirth()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(uVertex), &vertices[0], GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	// Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(uVertex), (void*)0);
	// Normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(uVertex), (void*)offsetof(uVertex, u_Normal));

	// Texture cords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(uVertex), (void*)offsetof(uVertex, u_TexCoords));

	glBindVertexArray(0);
}
