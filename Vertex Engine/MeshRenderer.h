#pragma once
#include "Vertex.h"
#include "VertexComponent.h"
#include "Material.h"
class MeshRenderer : public VertexComponent
{
public:
	void AssignMesh(MeshData& _mesh) { mesh = &_mesh; }
	// Called one to upload to gpu
	void Init();
	// Called by the Vertex Renderer
	void RenderMesh();
	// Render editor display
	void RenderEditorDisplay() override;
	// Add a material
	void AddMaterial(Material* _mat) { m_Materials.push_back(_mat); }
	// Get the list of materials
	std::vector<Material*> GetMaterials() { return m_Materials; }
private:
	std::vector<Material*> m_Materials;
	MeshData* mesh;
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;
};

