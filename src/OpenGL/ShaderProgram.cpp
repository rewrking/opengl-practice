#include "OpenGL/ShaderProgram.hpp"

namespace ogl
{
namespace
{
inline i32 getUniformLocation(u32 id, const char* inName)
{
	i32 location = glGetUniformLocation(id, inName);
	if (location < 0)
	{
		throw std::runtime_error(fmt::format("Error finding location of uniform '{}'", inName));
	}

	return location;
}
}

/*****************************************************************************/
[[nodiscard]] ShaderProgram ShaderProgram::make(const StringList& inShaderFiles)
{
	ShaderProgram ret;
	bool result = ret.loadFromFiles(inShaderFiles);
	if (!result)
	{
		throw std::runtime_error(std::string("Failed to load shader program!"));
	}
	return ret;
}

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
bool ShaderProgram::loadFromFiles(const StringList& inShaderFiles)
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
	i32 location = getUniformLocation(m_id, inName);
	glCheck(glUniform1f(location, inValue));
}

/*****************************************************************************/
void ShaderProgram::setUniform2f(const char* inName, f32 inX, f32 inY)
{
	i32 location = getUniformLocation(m_id, inName);
	glCheck(glUniform2f(location, inX, inY));
}

/*****************************************************************************/
void ShaderProgram::setUniform3f(const char* inName, f32 inX, f32 inY, f32 inZ)
{
	i32 location = getUniformLocation(m_id, inName);
	glCheck(glUniform3f(location, inX, inY, inZ));
}

/*****************************************************************************/
void ShaderProgram::setUniform4f(const char* inName, f32 inX, f32 inY, f32 inZ, f32 inW)
{
	i32 location = getUniformLocation(m_id, inName);
	glCheck(glUniform4f(location, inX, inY, inZ, inW));
}

/*****************************************************************************/
void ShaderProgram::setUniform4f(const char* inName, const Color& inColor)
{
	i32 location = getUniformLocation(m_id, inName);
	glCheck(glUniform4f(location, inColor.r, inColor.g, inColor.b, inColor.a));
}

/*****************************************************************************/
void ShaderProgram::setUniform1i(const char* inName, i32 inValue)
{
	i32 location = getUniformLocation(m_id, inName);
	glCheck(glUniform1i(location, inValue));
}

/*****************************************************************************/
void ShaderProgram::setUniformMatrix4f(const char* inName, const Mat4f& inValue)
{
	i32 location = getUniformLocation(m_id, inName);
	glCheck(glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(inValue)));
}

}

#undef CHECK_LOCATION
