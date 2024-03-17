#pragma once

#include "Libraries/OpenGL.hpp"

namespace ogl
{
struct ShaderStage;
using ShaderList = std::vector<ShaderStage>;

struct ShaderStage
{
	enum class Type
	{
		None,
		Vertex,
		Fragment,
		Geometry,
		Compute,
	};

	ShaderStage() = default;
	explicit ShaderStage(const std::string& inFilePath);

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
