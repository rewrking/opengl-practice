#include "OpenGL/ShaderStage.hpp"

#include "Core/Helpers.hpp"

#include <sstream>

namespace ogl
{
/*****************************************************************************/
ShaderStage::ShaderStage(const std::string& inFilePath) :
	m_filename(inFilePath)
{
}

/*****************************************************************************/
u32 ShaderStage::id() const noexcept
{
	return m_id;
}

/*****************************************************************************/
ShaderStage::Type ShaderStage::type() const noexcept
{
	return m_type;
}

/*****************************************************************************/
[[nodiscard]] std::string ShaderStage::readFile(const std::string& inFilePath)
{
	auto filename = ShaderStage::getFullPath(inFilePath);
	return readResolvedFile(filename);
}

/*****************************************************************************/
[[nodiscard]] ShaderStage::Type ShaderStage::getTypeFromPath(const std::string& inFilePath)
{
	if (String::endsWith(".vert", inFilePath))
		return Type::Vertex;

	if (String::endsWith(".frag", inFilePath))
		return Type::Fragment;

	if (String::endsWith(".geom", inFilePath))
		return Type::Geometry;

	if (String::endsWith(".comp", inFilePath))
		return Type::Compute;

	return Type::None;
}

/*****************************************************************************/
[[nodiscard]] std::string ShaderStage::readResolvedFile(const std::string& inFilePath)
{
	std::stringstream buffer;
	{
		std::error_code ec;
		if (!fs::exists(inFilePath, ec))
		{
			log_error("File doesn't exist:", inFilePath);
			return std::string();
		}

		std::ifstream t(inFilePath);
		buffer << t.rdbuf();
	}

	return buffer.str();
}

/*****************************************************************************/
[[nodiscard]] std::string ShaderStage::getFullPath(const std::string& inFilePath)
{
	return std::string("content/fx/") + inFilePath;
}

/*****************************************************************************/
bool ShaderStage::loadFromFile(const std::string& inFilePath)
{
	m_filename = ShaderStage::getFullPath(inFilePath);
	auto type = ShaderStage::getTypeFromPath(inFilePath);
	return loadFromSource(ShaderStage::readResolvedFile(m_filename), type);
}

/*****************************************************************************/
bool ShaderStage::loadFromSource(const std::string& inSource, const Type inType)
{
	if (inSource.empty())
		return false;

	auto shaderSourceData = inSource.c_str();

	auto getGLenumFromType = [](const Type lType) -> GLenum {
		switch (lType)
		{
			case Type::Vertex: return GL_VERTEX_SHADER;
			case Type::Fragment: return GL_FRAGMENT_SHADER;
			case Type::Geometry: return GL_GEOMETRY_SHADER;
			case Type::Compute: return GL_COMPUTE_SHADER;
			default: return GL_NONE;
		}
	};

	m_type = inType;

	GLenum type = getGLenumFromType(inType);
	if (type == GL_NONE || m_type == Type::None)
	{
		log_error("ShaderStage failed to load (type unknown)");
		return false;
	}

	m_id = glCreateShader(type);

	glCheck(glShaderSource(m_id, 1, &shaderSourceData, nullptr));
	glCheck(glCompileShader(m_id));

	i32 success;
	glCheck(glGetShaderiv(m_id, GL_COMPILE_STATUS, &success));
	if (success <= 0)
	{
		std::array<char, 512> infoLog;
		glCheck(glGetShaderInfoLog(m_id, static_cast<GLsizei>(infoLog.size()), nullptr, reinterpret_cast<GLchar*>(infoLog.data())));

		dispose();

		log_error("ShaderStage compilation failed:", infoLog.data());
		return false;
	}

	auto getStringFromType = [](const Type lType) -> std::string {
		switch (lType)
		{
			case Type::Vertex: return "Vertex";
			case Type::Fragment: return "Fragment";
			case Type::Geometry: return "Geometry";
			case Type::Compute: return "Compute";
			default: return "Unknown";
		}
	};

	auto typeString = getStringFromType(inType);
	if (!m_filename.empty())
	{
		log_info(fmt::format("Loaded {} ({})", m_filename, typeString));
	}
	else
	{
		log_info(fmt::format("Loaded shader from source ({})", typeString));
	}

	return true;
}

/*****************************************************************************/
void ShaderStage::dispose()
{
	if (m_id > 0)
	{
		glCheck(glDeleteShader(m_id));
		m_id = 0;
	}
}

}
