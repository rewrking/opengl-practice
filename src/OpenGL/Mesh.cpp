#include "OpenGL/Mesh.hpp"

#include "OpenGL/OpenGL.hpp"

namespace ogl
{
/*****************************************************************************/
Mesh::Mesh(const VertexList& inVertices, const IndexList& inIndices, const TextureList& inTextures) :
	vertices(inVertices),
	indices(inIndices),
	textures(inTextures)
{
	setupMesh();
}

/*****************************************************************************/
void Mesh::setupMesh() noexcept
{
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ebo);

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	constexpr i32 kVertexSize = sizeof(Vertex);
	constexpr auto kNormalOffset = offsetof(Vertex, normal);
	constexpr auto kTexCoordOffset = offsetof(Vertex, texCoords);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * kVertexSize, vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(u32), indices.data(), GL_STATIC_DRAW);

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, kVertexSize, (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, kVertexSize, (void*)kNormalOffset);
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, kVertexSize, (void*)kTexCoordOffset);

	glBindVertexArray(0);
}

/*****************************************************************************/
void Mesh::draw(Material& material)
{
	u32 diffuseNr = 1;
	u32 specularNr = 1;
	for (u32 i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
		// retrieve texture number (the N in diffuse_textureN)
		std::string number;
		const auto& name = textures[i].type;
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++);

		material.setInt(("material." + name + number).c_str(), i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}
	glActiveTexture(GL_TEXTURE0);

	// draw mesh
	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
}
