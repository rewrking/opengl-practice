#include "Core/ProgramBase.hpp"

namespace ogl
{
enum VAO_IDs
{
	Triangles,
	NumVAOs
};
enum Buffer_IDs
{
	ArrayBuffer,
	NumBuffers
};
enum Attrib_IDs
{
	vPosition = 0
};

constexpr GLuint numVertices = 6;

struct Program final : ProgramBase
{
	GLuint m_vaos[NumVAOs];
	GLuint m_buffers[NumBuffers];

	const GLfloat m_vertices[numVertices][2] = {
		// clang-format off
		{ -0.90f, -0.90f },
		{  0.85f, -0.90f },
		{ -0.90f,  0.85f },
		{  0.90f, -0.85f },
		{  0.90f,  0.90f },
		{ -0.85f,  0.90f },
		// clang-format on
	};

	virtual Settings getSettings() const final
	{
		return Settings("01: Triangles", 640, 480);
	}

	virtual void init() final
	{
		glCheck(glClearColor(100.0f / 255.0f, 149.0f / 255.0f, 237.0f / 255.0f, 1.0f));

		glCheck(glCreateVertexArrays(NumVAOs, m_vaos));
		glCheck(glCreateBuffers(NumBuffers, m_buffers));
		glCheck(glNamedBufferStorage(m_buffers[ArrayBuffer], sizeof(m_vertices), m_vertices, 0));

		std::vector<ShaderInfo> shaders = {
			{ GL_VERTEX_SHADER, getShader("triangles.vert") },
			{ GL_FRAGMENT_SHADER, getShader("triangles.frag") },
		};

		GLuint program = loadShaders(shaders);
		glCheck(glUseProgram(program));

		glCheck(glBindVertexArray(m_vaos[Triangles]));
		glCheck(glBindBuffer(GL_ARRAY_BUFFER, m_buffers[ArrayBuffer]));
		glCheck(glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0)));
		glCheck(glEnableVertexAttribArray(vPosition));
	}

	virtual void
	update() final
	{
		glCheck(glClear(GL_COLOR_BUFFER_BIT));
	}

	virtual void dispose() final
	{
	}
};
}

OGL_RUN_MAIN(ogl::Program);
