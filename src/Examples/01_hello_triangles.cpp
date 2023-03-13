#include "OpenGL/ProgramBase.hpp"

namespace ogl
{
namespace
{
f32 vertices[] = {
	// clang-format off
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
	// clang-format on
};
}
struct Program final : ProgramBase
{

	u32 m_vbo = 0;
	u32 m_vao = 0;

	ShaderProgram shaderProgram;

	virtual Settings getSettings() const final
	{
		return Settings("01: Hello Triangles", 800, 600);
	}

	virtual void init() final
	{
		glCheck(glClearColor(100.0f / 255.0f, 149.0f / 255.0f, 237.0f / 255.0f, 1.0f));

		shaderProgram = loadShaderProgram({
			"triangles.vert",
			"triangles.frag",
		});

		{
			glCheck(glGenVertexArrays(1, &m_vao));
			glCheck(glGenBuffers(1, &m_vbo));

			glCheck(glBindVertexArray(m_vao));

			glCheck(glBindBuffer(GL_ARRAY_BUFFER, m_vbo));
			glCheck(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

			GLuint attribLocation = 0;
			GLint attribSize = 3;
			glCheck(glVertexAttribPointer(attribLocation, attribSize, GL_FLOAT, GL_FALSE, attribSize * sizeof(float), (void*)0));
			glCheck(glEnableVertexAttribArray(attribLocation));

			glCheck(glBindBuffer(GL_ARRAY_BUFFER, 0));
			glCheck(glBindVertexArray(0));
		}

		// wireframe!
		// glCheck(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
	}

	virtual void update() final
	{
		glCheck(glClear(GL_COLOR_BUFFER_BIT));

		shaderProgram.use();
		glCheck(glBindVertexArray(m_vao));
		glCheck(glDrawArrays(GL_TRIANGLES, 0, 3));
	}

	virtual void dispose() final
	{
		glCheck(glDeleteVertexArrays(1, &m_vao));
		glCheck(glDeleteBuffers(1, &m_vbo));
		shaderProgram.dispose();
	}
};
}

OGL_RUN_MAIN(ogl::Program);
