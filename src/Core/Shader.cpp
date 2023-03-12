#include "Core/Shader.hpp"

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
bool Shader::loadFromFile(const std::string& inFilePath)
{
	auto shaderSource = readFile(getShaderPath(inFilePath.c_str()));
	auto shaderSourceData = shaderSource.c_str();

	GLuint type = 0;
	if (String::endsWith(inFilePath, ".vert"))
	{
		type = GL_VERTEX_SHADER;
	}
	else if (String::endsWith(inFilePath, ".frag"))
	{
		type = GL_FRAGMENT_SHADER;
	}
	else
	{
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
		glCheck(glGetShaderInfoLog(m_id, infoLog.size(), nullptr, reinterpret_cast<GLchar*>(infoLog.data())));

		dispose();

		throw std::runtime_error(std::string("Shader compilation failed:\n") + infoLog.data());
	}

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
			throw std::runtime_error("File doesn't exist!");
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
