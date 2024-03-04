#include "OpenGL/Mesh.hpp"

#include "Libraries/OpenGL.hpp"

namespace ogl
{
/*****************************************************************************/
bool Mesh::load()
{
	if (vertices.empty() || indices.empty())
	{
		log_error("Error loading mesh: empty");
		return false;
	}

	setupMesh();
	return true;
}

/*****************************************************************************/
void Mesh::setupMesh()
{
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ebo);

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	constexpr i32 kVertexSize = sizeof(Vertex3D);
	constexpr auto kNormalOffset = offsetof(Vertex3D, normal);
	constexpr auto kTexCoordOffset = offsetof(Vertex3D, texCoords);

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
		u32 number = 0;
		const auto& name = textures.at(i).type;
		if (name == "texture_diffuse")
			number = diffuseNr++;
		else if (name == "texture_specular")
			number = specularNr++;

		material.setInt(fmt::format("material.{}{}", name, number).c_str(), i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}
	glActiveTexture(GL_TEXTURE0);

	// draw mesh
	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
}
