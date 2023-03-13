#include "OpenGL/ShaderProgram.hpp"

#define CHECK_LOCATION()                                                                         \
	if (location < 0)                                                                            \
	{                                                                                            \
		throw std::runtime_error(fmt::format("Error finding location of uniform '{}'", inName)); \
	}

namespace ogl
{
/*****************************************************************************/
u32 ShaderProgram::id() const noexcept
{
	return m_id;
}

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

/*****************************************************************************/
void ShaderProgram::setUniform1f(const char* inName, f32 inValue)
{
	i32 location = glGetUniformLocation(m_id, inName);
	CHECK_LOCATION();
	glCheck(glUniform1f(location, inValue));
}

/*****************************************************************************/
void ShaderProgram::setUniform2f(const char* inName, f32 inX, f32 inY)
{
	i32 location = glGetUniformLocation(m_id, inName);
	CHECK_LOCATION();
	glCheck(glUniform2f(location, inX, inY));
}

/*****************************************************************************/
void ShaderProgram::setUniform3f(const char* inName, f32 inX, f32 inY, f32 inZ)
{
	i32 location = glGetUniformLocation(m_id, inName);
	CHECK_LOCATION();
	glCheck(glUniform3f(location, inX, inY, inZ));
}

/*****************************************************************************/
void ShaderProgram::setUniform4f(const char* inName, f32 inX, f32 inY, f32 inZ, f32 inW)
{
	i32 location = glGetUniformLocation(m_id, inName);
	CHECK_LOCATION();
	glCheck(glUniform4f(location, inX, inY, inZ, inW));
}
}

#undef CHECK_LOCATION
