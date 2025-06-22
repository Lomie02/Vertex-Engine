#include "MeshRenderer.h"

void MeshRenderer::Init()
{
	if (m_HasBeenUploaded) return;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(Vertex), &mesh.vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); 
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(uint32_t), &mesh.indices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoord));
	glEnableVertexAttribArray(2);


	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
	glEnableVertexAttribArray(3);

	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, BitTangent));
	glEnableVertexAttribArray(4);

	glBindVertexArray(0);
	m_HasBeenUploaded = true;
}

void MeshRenderer::RenderMesh()
{
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void MeshRenderer::RenderEditorDisplay()
{
	ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(1, 0.5f, 0, 1));
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1, 0.5f, 0, 1));
	if (ImGui::CollapsingHeader("Mesh Renderer", ImGuiTreeNodeFlags_DefaultOpen)) {
		ImGui::Text("Mesh Indices: "); ImGui::SameLine(); ImGui::Text(std::to_string(mesh.indices.size()).c_str());
		ImGui::Text("Mesh Vertices: "); ImGui::SameLine(); ImGui::Text(std::to_string(mesh.vertices.size()).c_str());


	}
	ImGui::PopStyleColor(2);
}
