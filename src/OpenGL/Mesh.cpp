#include "OpenGL/Mesh.hpp"

#include "Libraries/OpenGL.hpp"

namespace ogl
{
/*****************************************************************************/
bool Mesh::load()
{
#if OGL_USE_INDICES
	if (vertices.empty() || indices.empty())
#else
	if (vertices.empty())
#endif
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
	textures.clear();
	vertices.clear();

#if OGL_USE_INDICES
	indices.clear();

	if (m_ebo > 0)
	{
		glCheck(glDeleteBuffers(1, &m_ebo));
		m_ebo = 0;
	}
#endif

	if (m_vbo > 0)
	{
		glCheck(glDeleteBuffers(1, &m_vbo));
		m_vbo = 0;
	}

	if (m_vao > 0)
	{
		glCheck(glDeleteVertexArrays(1, &m_vao));
		m_vao = 0;
	}
}

/*****************************************************************************/
void Mesh::setupMesh()
{
	glCheck(glGenVertexArrays(1, &m_vao));
	glCheck(glGenBuffers(1, &m_vbo));

#if OGL_USE_INDICES
	glCheck(glGenBuffers(1, &m_ebo));
#endif

	glCheck(glBindVertexArray(m_vao));
	glCheck(glBindBuffer(GL_ARRAY_BUFFER, m_vbo));

	constexpr i32 kVertexSize = sizeof(VertexType);
	constexpr auto kNormalOffset = offsetof(VertexType, normal);
	constexpr auto kTexCoordOffset = offsetof(VertexType, texCoords);
	constexpr auto kTangentOffset = offsetof(VertexType, tangent);
	constexpr auto kBitangentOffset = offsetof(VertexType, bitangent);

	glCheck(glBufferData(GL_ARRAY_BUFFER, kVertexSize * vertices.size(), vertices.data(), GL_STATIC_DRAW));

#if OGL_USE_INDICES
	constexpr i32 kIndexSize = sizeof(IndexType);
	glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo));
	glCheck(glBufferData(GL_ELEMENT_ARRAY_BUFFER, kIndexSize * indices.size(), indices.data(), GL_STATIC_DRAW));

#endif

	// vertex positions
	glCheck(glEnableVertexAttribArray(0));
	glCheck(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, kVertexSize, (void*)0));

	// vertex normals
	glCheck(glEnableVertexAttribArray(1));
	glCheck(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, kVertexSize, (void*)kNormalOffset));

	// vertex texture coords
	glCheck(glEnableVertexAttribArray(2));
	glCheck(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, kVertexSize, (void*)kTexCoordOffset));

	// vertex tangent
	glCheck(glEnableVertexAttribArray(3));
	glCheck(glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, kVertexSize, (void*)kTangentOffset));

	// vertex bitangent
	glCheck(glEnableVertexAttribArray(4));
	glCheck(glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, kVertexSize, (void*)kBitangentOffset));

	glCheck(glBindVertexArray(0));
}

/*****************************************************************************/
void Mesh::draw(ShaderProgram& material) const
{
	u32 diffuseIdx = 0;
	u32 specularIdx = 0;
	u32 normalIdx = 0;
	u32 heightIdx = 0;
	for (u32 i = 0; i < textures.size(); ++i)
	{
		u32 number = 0;
		std::string id;
		auto& texture = textures.at(i);
		if (texture->kind == TextureKind::Diffuse)
		{
			number = diffuseIdx++;
			id = "diffuse";
		}
		else if (texture->kind == TextureKind::Specular)
		{
			number = specularIdx++;
			id = "specular";
		}
		else if (texture->kind == TextureKind::Normal)
		{
			number = normalIdx++;
			id = "normal";
		}
		else if (texture->kind == TextureKind::Height)
		{
			number = heightIdx++;
			id = "height";
		}
		else
		{
			return;
		}

		UNUSED(number);
		if (texture->buffer.loaded())
		{
			texture->buffer.bind(static_cast<i32>(i));
			// material.setTexture(fmt::format("u_Material.{}[{}]", id, number).c_str(), texture->buffer);
			material.setTexture(fmt::format("u_Material.{}", id).c_str(), texture->buffer);
		}
	}

	// draw mesh
	glCheck(glBindVertexArray(m_vao));

#if OGL_USE_INDICES
	glCheck(glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0));
#else
	glCheck(glDrawArrays(GL_TRIANGLES, 0, static_cast<i32>(vertices.size())));
#endif

	glCheck(glBindVertexArray(0));

	// always good practice to set everything back to defaults once configured.
	glCheck(glActiveTexture(GL_TEXTURE0));
}
}
