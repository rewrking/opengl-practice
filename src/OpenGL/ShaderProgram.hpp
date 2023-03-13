#pragma once

#include "OpenGL/OpenGL.hpp"
#include "OpenGL/Shader.hpp"

namespace ogl
{
struct ShaderProgram
{
	ShaderProgram() = default;

	bool valid() const noexcept;

	bool load(const StringList& inShaderFiles);

	void dispose();

private:
	u32 m_id = 0;
};
}
