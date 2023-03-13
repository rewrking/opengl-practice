#include "OpenGL/Shader.hpp"

#include "Core/Helpers.hpp"

#include <sstream>

namespace ogl
{
/*****************************************************************************/
u32 Shader::id() const noexcept
{
	return m_id;
}

/*****************************************************************************/
Shader::Type Shader::type() const noexcept
{
	return m_type;
}

/*****************************************************************************/
bool Shader::loadFromFile(const std::string& inFilePath)
{
	auto resolvedPath = getShaderPath(inFilePath.c_str());
	auto shaderSource = readFile(resolvedPath);
	if (shaderSource.empty())
		return false;

	auto shaderSourceData = shaderSource.c_str();

	GLenum type = 0;
	if (String::endsWith(".vert", inFilePath))
	{
		type = GL_VERTEX_SHADER;
		m_type = Type::Vertex;
	}
	else if (String::endsWith(".frag", inFilePath))
	{
		type = GL_FRAGMENT_SHADER;
		m_type = Type::Fragment;
	}
	else
	{
		log_error("Unknown type:", inFilePath);
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

		log_error("Shader compilation failed:", infoLog.data());
		return false;
	}

	auto typeString = (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment");
	log_info(fmt::format("Loaded {} ({})", resolvedPath, typeString));

	return true;
}

/*****************************************************************************/
void Shader::dispose()
{
	if (m_id > 0)
	{
		glCheck(glDeleteShader(m_id));
		m_id = 0;
	}
}

/*****************************************************************************/
std::string Shader::readFile(const std::string& inFilePath) const
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
std::string Shader::getShaderPath(const char* inPath) const
{
	return std::string("content/fx/") + inPath;
}

}
