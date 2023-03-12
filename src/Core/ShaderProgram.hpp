#pragma once

#include "Core/OpenGL.hpp"
#include "Core/Shader.hpp"

namespace ogl
{
struct ShaderProgram
{
	ShaderProgram();

	bool load(const StringList& inShaderFiles);

	void dispose();

private:
	std::vector<Shader> m_shaders;

	u32 m_id = 0;
};
}
