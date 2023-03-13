#include "OpenGL/ProgramBase.hpp"

namespace ogl
{
struct Program final : ProgramBase
{
	const std::vector<f32> m_vertices = {
		// clang-format off
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f
		// clang-format on
	};

	u32 m_vbo = 0;

	ShaderProgram shaderProgram;

	virtual Settings getSettings() const final
	{
		return Settings("01: Hello Triangles", 800, 600);
	}

	virtual void init() final
	{
		glCheck(glClearColor(100.0f / 255.0f, 149.0f / 255.0f, 237.0f / 255.0f, 1.0f));

		glCheck(glGenBuffers(1, &m_vbo));
		glCheck(glBindBuffer(GL_ARRAY_BUFFER, m_vbo));
		glCheck(glBufferData(GL_ARRAY_BUFFER, m_vertices.size(), m_vertices.data(), GL_STATIC_DRAW));

		shaderProgram = loadShaderProgram({
			"triangles.vert",
			"triangles.frag",
		});
	}

	virtual void update() final
	{
		glCheck(glClear(GL_COLOR_BUFFER_BIT));
	}

	virtual void dispose() final
	{
		shaderProgram.dispose();
	}
};
}

OGL_RUN_MAIN(ogl::Program);
