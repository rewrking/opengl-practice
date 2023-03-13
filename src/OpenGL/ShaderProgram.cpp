#include "OpenGL/ShaderProgram.hpp"

namespace ogl
{
/*****************************************************************************/
bool ShaderProgram::valid() const noexcept
{
	return m_id > 0;
}

/*****************************************************************************/
bool ShaderProgram::load(const StringList& inShaderFiles)
{
	dispose();

	std::vector<Shader> shaders;
	auto disposeShaders = [&shaders]() -> bool {
		for (auto& shader : shaders)
			shader.dispose();

		shaders.clear();
		return false;
	};

	for (auto& file : inShaderFiles)
	{
		Shader shader;
		bool result = shader.loadFromFile(file);
		if (!result)
			return disposeShaders();

		shaders.emplace_back(std::move(shader));
	}

	std::vector<Shader::Type> shaderTypes;

	m_id = glCreateProgram();
	for (auto& shader : shaders)
	{
		auto type = shader.type();
		for (auto shaderType : shaderTypes)
		{
			if (type == shaderType)
			{
				log_error("Shader program cannot contain duplicate shader types");
				return disposeShaders();
			}
		}
		shaderTypes.emplace_back(shader.type());

		glCheck(glAttachShader(m_id, shader.id()));
	}

	glCheck(glLinkProgram(m_id));

	i32 success = 0;
	glCheck(glGetProgramiv(m_id, GL_LINK_STATUS, &success));
	if (success == GL_FALSE)
	{
		std::array<char, 512> infoLog;
		glCheck(glGetProgramInfoLog(m_id, static_cast<GLsizei>(infoLog.size()), nullptr, infoLog.data()));

		dispose();

		log_error("Shader program linking failed:", infoLog.data());
		return disposeShaders();
	}

	disposeShaders();

	return true;
}

/*****************************************************************************/
void ShaderProgram::use() const
{
	glCheck(glUseProgram(m_id));
}

/*****************************************************************************/
void ShaderProgram::dispose()
{
	if (m_id > 0)
	{
		glCheck(glDeleteProgram(m_id));
		m_id = 0;
	}
}
}
