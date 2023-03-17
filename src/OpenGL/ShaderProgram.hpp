#pragma once

#include "OpenGL/GLM.hpp"
#include "OpenGL/OpenGL.hpp"
#include "OpenGL/Shader.hpp"

namespace ogl
{
struct ShaderProgram
{
	ShaderProgram() = default;

	[[nodiscard]] static ShaderProgram make(const StringList& inShaderFiles);

	u32 id() const noexcept;
	bool valid() const noexcept;

	void use() const;

	void dispose();

	void setUniform1f(const char* inName, f32 inValue);
	void setUniform2f(const char* inName, f32 inX, f32 inY);
	void setUniform3f(const char* inName, f32 inX, f32 inY, f32 inZ);
	void setUniform4f(const char* inName, f32 inX, f32 inY, f32 inZ, f32 inW);

	void setUniform1i(const char* inName, i32 inValue);

	void setUniformMatrix4f(const char* inName, const glm::mat4& inValue);

private:
	bool loadFromFiles(const StringList& inShaderFiles);

	u32 m_id = 0;
};
}
