#include "OpenGL/Material.hpp"

#include "OpenGL/Mesh.hpp"

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
const Material* Material::kCurrentMaterial = nullptr;

/*****************************************************************************/
u32 Material::id() const noexcept
{
	return m_id;
}

/*****************************************************************************/
bool Material::valid() const noexcept
{
	return m_id > 0;
}

/*****************************************************************************/
bool Material::loadFromFiles(const StringList& inShaderFiles)
{
	dispose();

	std::vector<Shader> shaders;
	auto disposeShaders = [&shaders]() -> void {
		for (auto& shader : shaders)
			shader.dispose();

		shaders.clear();
	};

	for (auto& file : inShaderFiles)
	{
		Shader shader;
		bool result = shader.loadFromFile(file);
		if (!result)
		{
			log_error("Failed to load shader program:", file);
			disposeShaders();
			return false;
		}

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
				disposeShaders();
				return false;
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
		disposeShaders();
		return false;
	}

	disposeShaders();

	return true;
}

/*****************************************************************************/
void Material::bind() const
{
	if (kCurrentMaterial != this)
	{
		glCheck(glUseProgram(m_id));
		kCurrentMaterial = this;
	}
}

/*****************************************************************************/
void Material::dispose()
{
	if (kCurrentMaterial == this)
		kCurrentMaterial = nullptr;

	if (m_id > 0)
	{
		glCheck(glDeleteProgram(m_id));
		m_id = 0;
	}
}

/*****************************************************************************/
void Material::setUniform1f(const char* inName, f32 inValue)
{
	this->bind();
	i32 location = getUniformLocation(m_id, inName);
	glCheck(glUniform1f(location, inValue));
}

/*****************************************************************************/
void Material::setUniform2f(const char* inName, f32 inX, f32 inY)
{
	this->bind();
	i32 location = getUniformLocation(m_id, inName);
	glCheck(glUniform2f(location, inX, inY));
}

/*****************************************************************************/
void Material::setUniform2f(const char* inName, const Vec2f& inVec)
{
	this->bind();
	i32 location = getUniformLocation(m_id, inName);
	glCheck(glUniform2f(location, inVec.x, inVec.y));
}

/*****************************************************************************/
void Material::setUniform3f(const char* inName, f32 inX, f32 inY, f32 inZ)
{
	this->bind();
	i32 location = getUniformLocation(m_id, inName);
	glCheck(glUniform3f(location, inX, inY, inZ));
}

/*****************************************************************************/
void Material::setUniform3f(const char* inName, const Vec3f& inVec)
{
	this->bind();
	i32 location = getUniformLocation(m_id, inName);
	glCheck(glUniform3f(location, inVec.x, inVec.y, inVec.z));
}

/*****************************************************************************/
void Material::setUniform4f(const char* inName, f32 inX, f32 inY, f32 inZ, f32 inW)
{
	this->bind();
	i32 location = getUniformLocation(m_id, inName);
	glCheck(glUniform4f(location, inX, inY, inZ, inW));
}

/*****************************************************************************/
void Material::setUniform4f(const char* inName, const Color& inColor)
{
	this->bind();
	i32 location = getUniformLocation(m_id, inName);
	glCheck(glUniform4f(location, inColor.r, inColor.g, inColor.b, inColor.a));
}

/*****************************************************************************/
void Material::setUniform1i(const char* inName, i32 inValue)
{
	this->bind();
	i32 location = getUniformLocation(m_id, inName);
	glCheck(glUniform1i(location, inValue));
}
/*****************************************************************************/
void Material::setUniformMatrix3f(const char* inName, const Mat3f& inValue)
{
	this->bind();
	i32 location = getUniformLocation(m_id, inName);
	glCheck(glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(inValue)));
}

/*****************************************************************************/
void Material::setUniformMatrix4f(const char* inName, const Mat4f& inValue)
{
	this->bind();
	i32 location = getUniformLocation(m_id, inName);
	glCheck(glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(inValue)));
}

}

#undef CHECK_LOCATION
