#include "OpenGL/ProgramBase.hpp"

#include "OpenGL/BufferAttribList.hpp"

namespace ogl::Program
{
struct Textures final : ogl::ProgramBase
{
	const std::vector<f32> m_vertices = {
		// clang-format off
		-0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f, 1.0f,   1.0f, 0.0f,
		 0.0f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f, 1.0f,   0.5f, 1.0f,
		//  0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f, 1.0f,  // top right
		//  0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f, 1.0f,  // bottom right
		// -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f, 1.0f,  // bottom left
		// -0.5f,  0.5f, 0.0f,   0.0f, 0.0f, 0.0f, 1.0f,  // top left
		// clang-format on
	};
	const std::vector<u32> m_indices = {
		// clang-format off
		0, 1, 2,
		// 0, 1, 3,   // first triangle
		// 1, 2, 3    // second triangle
		// clang-format on
	};

	u32 m_vbo = 0;
	u32 m_ebo = 0;

	u32 m_vao = 0;

	u32 m_texture = 0;

	Material shaderProgram;

	virtual Settings getSettings() const final
	{
		return Settings("03: Textures", 800, 600);
	}

	virtual void init() final
	{
		setClearColor(100, 149, 237);
		setWireframe(false);

		shaderProgram.loadFromFile("03_textures.glsl");

		{
			auto image = Image::make("wall.jpg");

			glCheck(glGenTextures(1, &m_texture));
			glCheck(glBindTexture(GL_TEXTURE_2D, m_texture));

			/*
				GL_REPEAT
				GL_MIRRORED_REPEAT
				GL_CLAMP_TO_EDGE
				GL_CLAMP_TO_BORDER
			*/
			glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
			glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

			/*
				GL_NEAREST
				GL_LINEAR

			// control mipmaps levels
				GL_NEAREST_MIPMAP_NEAREST
				GL_LINEAR_MIPMAP_NEAREST
				GL_NEAREST_MIPMAP_LINEAR
				GL_LINEAR_MIPMAP_LINEAR
			*/
			glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
			glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

			glCheck(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, static_cast<GLsizei>(image.width), static_cast<GLsizei>(image.height), 0, GL_RGB, GL_UNSIGNED_BYTE, image.pixels.data()));
			glCheck(glGenerateMipmap(GL_TEXTURE_2D));
		};
		{
			glCheck(glGenVertexArrays(1, &m_vao));
			glCheck(glGenBuffers(1, &m_vbo));
			glCheck(glGenBuffers(1, &m_ebo));

			glCheck(glBindVertexArray(m_vao));

			glCheck(glBindBuffer(GL_ARRAY_BUFFER, m_vbo));
			glCheck(glBufferData(GL_ARRAY_BUFFER, sizeof(f32) * m_vertices.size(), m_vertices.data(), GL_STATIC_DRAW));

			glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo));
			glCheck(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(u32) * m_indices.size(), m_indices.data(), GL_STATIC_DRAW));

			{
				BufferAttribList attribList({ Attrib::Position3D, Attrib::ColorRGBA, Attrib::TexCoord });
				for (auto& attrib : attribList.attribs)
				{
					glCheck(glVertexAttribPointer(attrib.position, attrib.size, GL_FLOAT, GL_FALSE, sizeof(f32) * attribList.size, (void*)(attrib.offset * sizeof(f32))));
					glCheck(glEnableVertexAttribArray(attrib.position));
				}
			}

			glCheck(glBindBuffer(GL_ARRAY_BUFFER, 0));

			// no!
			// glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

			glCheck(glBindVertexArray(0));

			shaderProgram.bind();
			shaderProgram.setVec4("u_Color", 1.0f, 1.0f, 1.0f, 1.0f); // white
			shaderProgram.setInt("u_Texture", 0);
		}
	}

	virtual void update() final
	{
		clearContext();

		shaderProgram.bind();

		// f32 timeValue = glfwGetTime();
		// f32 greenValue = (std::sin(timeValue) / 2.0f) + 0.5f;
		// shaderProgram.setVec4("u_Color", 0.0f, greenValue, 0.0f, 1.0f);

		glCheck(glActiveTexture(GL_TEXTURE0));
		glCheck(glBindTexture(GL_TEXTURE_2D, m_texture));

		glCheck(glBindVertexArray(m_vao));
		// glCheck(glDrawArrays(GL_TRIANGLES, 0, 6));
		glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo));
		glCheck(glDrawElements(GL_TRIANGLES, static_cast<i32>(m_indices.size()), GL_UNSIGNED_INT, 0));
		glCheck(glBindVertexArray(0));
	}

	virtual void dispose() final
	{
		glCheck(glDeleteTextures(1, &m_texture));
		glCheck(glDeleteVertexArrays(1, &m_vao));
		glCheck(glDeleteBuffers(1, &m_vbo));
		glCheck(glDeleteBuffers(1, &m_ebo));
		shaderProgram.dispose();
	}
};
}
