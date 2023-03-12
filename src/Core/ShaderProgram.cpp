#include "Core/ShaderProgram.hpp"

namespace ogl
{
/*****************************************************************************/
ShaderProgram::ShaderProgram()
{
}

/*****************************************************************************/
bool ShaderProgram::load(const StringList& inShaderFiles)
{
	dispose();

	for (auto& file : inShaderFiles)
	{
		Shader shader;
		bool result = shader.loadFromFile(file);
		if (!result)
		{
			throw std::runtime_error(std::string("Failed to load shader: ") + file);
		}

		m_shaders.emplace_back(std::move(shader));
	}

	m_id = glCreateProgram();
	for (auto& shader : m_shaders)
	{
		glCheck(glAttachShader(m_id, shader.id()));
	}

	glCheck(glLinkProgram(m_id));

	i32 success = 0;
	glCheck(glGetProgramiv(m_id, GL_LINK_STATUS, &success));
	if (success == GL_FALSE)
	{
		std::array<char, 512> infoLog;
		glCheck(glGetProgramInfoLog(m_id, infoLog.size(), nullptr, infoLog.data()));

		dispose();

		throw std::runtime_error(std::string("Shader program linking failed:\n") + infoLog.data());
	}

	glCheck(glUseProgram(m_id));

	return true;
}

/*****************************************************************************/
void ShaderProgram::dispose()
{
	if (m_id > 0)
	{
		glCheck(glDeleteProgram(m_id));
		m_id = 0;
	}

	for (auto& shader : m_shaders)
	{
		shader.dispose();
	}
}
}
