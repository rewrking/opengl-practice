#pragma once

#include "OpenGL/OpenGL.hpp"

namespace ogl
{
struct Shader;
using ShaderList = std::vector<Shader>;

struct Shader
{
	enum class Type
	{
		None,
		Vertex,
		Fragment,
		Geometry,
		Compute,
	};

	Shader() = default;
	explicit Shader(const std::string& inFilePath);

	u32 id() const noexcept;
	Type type() const noexcept;

	[[nodiscard]] static std::string readFile(const std::string& inFilePath);
	[[nodiscard]] static Type getTypeFromPath(const std::string& inFilePath);

	bool loadFromFile(const std::string& inFilePath);
	bool loadFromSource(const std::string& inSource, const Type inType);
	void dispose();

private:
	[[nodiscard]] static std::string readResolvedFile(const std::string& inFilePath);
	[[nodiscard]] static std::string getFullPath(const std::string& inFilePath);

	std::string m_filename;

	u32 m_id = 0;
	Type m_type = Type::None;
};

}
