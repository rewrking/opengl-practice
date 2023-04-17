#include "OpenGL/Mesh.hpp"

#include "OpenGL/BufferAttribList.hpp"
#include "OpenGL/Material.hpp"
#include "OpenGL/OpenGL.hpp"

namespace ogl
{
/*****************************************************************************/
Mesh& Mesh::setGeometry(const std::vector<MeshAttribute>& inAttribs, std::vector<f32>&& inData)
{
	if (inData.empty() || inAttribs.empty())
		return *this;

	initialize();

	if (m_vao > 0 && m_vbo > 0)
	{
		m_data = std::move(inData);

		glCheck(glBindVertexArray(m_vao));

		glCheck(glBindBuffer(GL_ARRAY_BUFFER, m_vbo));
		glCheck(glBufferData(GL_ARRAY_BUFFER, sizeof(f32) * m_data.size(), m_data.data(), GL_STATIC_DRAW));

		// glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo));
		// glCheck(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(u32) * m_indices.size(), m_indices.data(), GL_STATIC_DRAW));

		BufferAttribList attribList(inAttribs);
		for (auto& attrib : attribList.attribs)
		{
			glCheck(glVertexAttribPointer(attrib.position, attrib.size, GL_FLOAT, GL_FALSE, sizeof(f32) * attribList.size, (void*)(attrib.offset * sizeof(f32))));
			glCheck(glEnableVertexAttribArray(attrib.position));
		}

		glCheck(glBindBuffer(GL_ARRAY_BUFFER, 0));

		// no!
		// glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

		glCheck(glBindVertexArray(0));
	}

	return *this;
}

/*****************************************************************************/
Mesh& Mesh::setGeometry(const std::vector<MeshAttribute>& inAttribs, const std::vector<f32>& inData)
{
	if (inData.empty() || inAttribs.empty())
		return *this;

	initialize();

	if (m_vao > 0 && m_vbo > 0)
	{
		m_data = inData;

		glCheck(glBindVertexArray(m_vao));

		glCheck(glBindBuffer(GL_ARRAY_BUFFER, m_vbo));
		glCheck(glBufferData(GL_ARRAY_BUFFER, sizeof(f32) * m_data.size(), m_data.data(), GL_STATIC_DRAW));

		// glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo));
		// glCheck(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(u32) * m_indices.size(), m_indices.data(), GL_STATIC_DRAW));

		BufferAttribList attribList(inAttribs);
		for (auto& attrib : attribList.attribs)
		{
			glCheck(glVertexAttribPointer(attrib.position, attrib.size, GL_FLOAT, GL_FALSE, sizeof(f32) * attribList.size, (void*)(attrib.offset * sizeof(f32))));
			glCheck(glEnableVertexAttribArray(attrib.position));
		}

		glCheck(glBindBuffer(GL_ARRAY_BUFFER, 0));

		// no!
		// glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

		glCheck(glBindVertexArray(0));
	}

	return *this;
}

/*****************************************************************************/
void Mesh::setMaterial(const Material& inMaterial)
{
	m_material = &inMaterial;
}

/*****************************************************************************/
void Mesh::initialize()
{
	if (m_vao == 0)
	{
		glCheck(glGenVertexArrays(1, &m_vao));
	}

	if (m_vbo == 0)
	{
		glCheck(glGenBuffers(1, &m_vbo));
	}

	// if (m_ebo == 0)
	// {
	// 	glCheck(glGenBuffers(1, &m_ebo));
	// }
}

/*****************************************************************************/
void Mesh::draw() const
{
	if (m_material != nullptr)
	{
		m_material->bind();
	}
	else
	{
		// We have no shader!
		log_error("draw() called on a mesh without a shader:", m_vao);
		return;
	}

	if (m_vao > 0)
	{
		glCheck(glBindVertexArray(m_vao));

		glCheck(glDrawArrays(GL_TRIANGLES, 0, static_cast<i32>(m_data.size())));

		glCheck(glBindVertexArray(0));
	}
}

/*****************************************************************************/
u32 Mesh::vbo() const
{
	return m_vbo;
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

	// if (m_ebo > 0)
	// {
	// 	glCheck(glDeleteBuffers(1, &m_ebo));
	// 	m_ebo = 0;
	// }

	if (m_material != nullptr)
	{
		m_material = nullptr;
	}
}
}
