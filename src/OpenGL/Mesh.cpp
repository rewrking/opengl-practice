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
void Mesh::dispose()
{
	if (m_vao > 0)
	{
		glCheck(glDeleteVertexArrays(1, &m_vao));
		m_vao = 0;
	}

	if (m_vbo > 0)
	{
		glCheck(glDeleteBuffers(1, &m_vbo));
		m_vbo = 0;
	}

	if (m_ebo > 0)
	{
		glCheck(glDeleteBuffers(1, &m_ebo));
		m_ebo = 0;
	}

	textures.clear();
	vertices.clear();
	indices.clear();
}

/*****************************************************************************/
void Mesh::setupMesh()
{
	glCheck(glGenVertexArrays(1, &m_vao));
	glCheck(glGenBuffers(1, &m_vbo));
	glCheck(glGenBuffers(1, &m_ebo));

	glCheck(glBindVertexArray(m_vao));
	glCheck(glBindBuffer(GL_ARRAY_BUFFER, m_vbo));

	constexpr i32 kVertexSize = sizeof(VertexType);
	constexpr i32 kIndexSize = sizeof(IndexType);
	constexpr auto kNormalOffset = offsetof(VertexType, normal);
	constexpr auto kTexCoordOffset = offsetof(VertexType, texCoords);

	glCheck(glBufferData(GL_ARRAY_BUFFER, vertices.size() * kVertexSize, vertices.data(), GL_STATIC_DRAW));

	glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo));
	glCheck(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * kIndexSize, indices.data(), GL_STATIC_DRAW));

	// vertex positions
	glCheck(glEnableVertexAttribArray(0));
	glCheck(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, kVertexSize, (void*)0));

	// vertex normals
	glCheck(glEnableVertexAttribArray(1));
	glCheck(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, kVertexSize, (void*)kNormalOffset));

	// vertex texture coords
	glCheck(glEnableVertexAttribArray(2));
	glCheck(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, kVertexSize, (void*)kTexCoordOffset));

	glCheck(glBindVertexArray(0));
}

/*****************************************************************************/
void Mesh::draw(Material& material) const
{
	u32 diffuseNr = 0;
	u32 specularNr = 0;
	for (u32 i = 0; i < textures.size(); ++i)
	{
		u32 number = 0;
		std::string id;
		auto& texture = textures.at(i);
		if (texture->kind == TextureKind::Diffuse)
		{
			number = diffuseNr++;
			id = "diffuse";
		}
		else if (texture->kind == TextureKind::Specular)
		{
			number = specularNr++;
			id = "specular";
		}
		else
		{
			return;
		}

		UNUSED(number);
		texture->buffer.bind(i);
		// material.setTexture(fmt::format("u_Material.{}[{}]", id, number).c_str(), texture->buffer);
		material.setTexture(fmt::format("u_Material.{}", id).c_str(), texture->buffer);
	}

	// draw mesh
	glCheck(glBindVertexArray(m_vao));
	glCheck(glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0));
	glCheck(glBindVertexArray(0));

	// always good practice to set everything back to defaults once configured.
	glCheck(glActiveTexture(GL_TEXTURE0));
}
}
