#pragma once
#include <vector>
#include "Graphics.h"
#include "glm.hpp"
struct Vertex {
	glm::vec3 position;
	glm::vec3 Normal;
	glm::vec2 TexCoord;
	glm::vec3 Tangent;
	glm::vec3 BitTangent;
};

struct MeshData {
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

};