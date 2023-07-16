#include "OpenGL/ProgramBase.hpp"

namespace ogl
{
struct Program final : ProgramBase
{
	const std::vector<f32> m_vertices = {
		// clang-format off
		// -0.5f, -0.5f, 0.0f,
		// 0.5f, -0.5f, 0.0f,
		// 0.0f,  0.5f, 0.0f
		 0.5f,  0.5f, 0.0f,  // top right
		 0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left
		// clang-format on
	};
	const std::vector<u32> m_indices = {
		// clang-format off
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
		// clang-format on
	};

	u32 m_vbo = 0;
	u32 m_ebo = 0;

	u32 m_vao = 0;

	Material shaderProgram;

	virtual Settings getSettings() const final
	{
		return Settings("01: Hello Triangles", 800, 600);
	}

	virtual void init() final
	{
		setClearColor(100, 149, 237);
		setWireframe(false);

		shaderProgram.loadFromFile("01_basic.glsl");

		{
			glCheck(glGenVertexArrays(1, &m_vao));
			glCheck(glGenBuffers(1, &m_vbo));
			glCheck(glGenBuffers(1, &m_ebo));

			glCheck(glBindVertexArray(m_vao));

			glCheck(glBindBuffer(GL_ARRAY_BUFFER, m_vbo));
			glCheck(glBufferData(GL_ARRAY_BUFFER, sizeof(f32) * m_vertices.size(), m_vertices.data(), GL_STATIC_DRAW));

			glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo));
			glCheck(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(u32) * m_indices.size(), m_indices.data(), GL_STATIC_DRAW));

			GLuint attribLocation = 0;
			GLint attribSize = 3;
			glCheck(glVertexAttribPointer(attribLocation, attribSize, GL_FLOAT, GL_FALSE, attribSize * sizeof(f32), (void*)0));
			glCheck(glEnableVertexAttribArray(attribLocation));

			glCheck(glBindBuffer(GL_ARRAY_BUFFER, 0));

			// no!
			// glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

			glCheck(glBindVertexArray(0));
		}
	}

	virtual void update() final
	{
		clearContext();

		shaderProgram.bind();
		glCheck(glBindVertexArray(m_vao));
		// glCheck(glDrawArrays(GL_TRIANGLES, 0, 6));
		glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo));
		glCheck(glDrawElements(GL_TRIANGLES, static_cast<i32>(m_indices.size()), GL_UNSIGNED_INT, 0));
		glCheck(glBindVertexArray(0));
	}

	virtual void dispose() final
	{
		glCheck(glDeleteVertexArrays(1, &m_vao));
		glCheck(glDeleteBuffers(1, &m_vbo));
		glCheck(glDeleteBuffers(1, &m_ebo));
		shaderProgram.dispose();
	}
};
}

OGL_RUN_MAIN(ogl::Program);
