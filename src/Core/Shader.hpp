#pragma once

#include "Core/OpenGL.hpp"

namespace ogl
{
struct Shader
{
	Shader() = default;

	u32 id() const noexcept;

	bool loadFromFile(const std::string& inFilePath);
	void dispose();

private:
	std::string readFile(const std::string& inFilePath) const;
	std::string getShaderPath(const char* inPath) const;

	u32 m_id = 0;
};
}
