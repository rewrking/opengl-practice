#pragma once

#include "OpenGL/OpenGL.hpp"
#include "OpenGL/Shader.hpp"

namespace ogl
{
struct ShaderProgram
{
	ShaderProgram() = default;

	u32 id() const noexcept;
	bool valid() const noexcept;

	bool load(const StringList& inShaderFiles);
	void use() const;

	void dispose();

	void setUniform1f(const char* inName, f32 inValue);
	void setUniform2f(const char* inName, f32 inX, f32 inY);
	void setUniform3f(const char* inName, f32 inX, f32 inY, f32 inZ);
	void setUniform4f(const char* inName, f32 inX, f32 inY, f32 inZ, f32 inW);

private:
	u32 m_id = 0;
};
}
