#pragma once

#include "OpenGL/OpenGL.hpp"

namespace ogl
{
struct Shader
{
	enum class Type
	{
		Vertex,
		Fragment,
	};

	Shader() = default;

	u32 id() const noexcept;
	Type type() const noexcept;

	bool loadFromFile(const std::string& inFilePath);
	void dispose();

private:
	std::string readFile(const std::string& inFilePath) const;
	std::string getShaderPath(const char* inPath) const;

	u32 m_id = 0;
	Type m_type = Type::Vertex;
};
}
