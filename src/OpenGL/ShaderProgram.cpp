#include "OpenGL/ShaderProgram.hpp"

#include "Core/Helpers.hpp"
#include "OpenGL/MeshOld.hpp"
#include "OpenGL/TextureBuffer.hpp"

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
const ShaderProgram* ShaderProgram::kCurrentMaterial = nullptr;

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

	ShaderList shaders;
	auto disposeShaders = [&shaders]() -> void {
		for (auto& shader : shaders)
			shader.dispose();

		shaders.clear();
	};

	for (auto& file : inShaderFiles)
	{
		ShaderStage shader;
		bool result = shader.loadFromFile(file);
		if (!result)
		{
			log_error("Failed to load material:", file);
			disposeShaders();
			return false;
		}

		shaders.emplace_back(std::move(shader));
	}

	bool result = loadFromShaders(shaders);
	disposeShaders();
	return result;
}

/*****************************************************************************/
bool ShaderProgram::loadFromFile(const std::string& inFile)
{
	if (!String::endsWith(".glsl", inFile))
	{
		log_error("ShaderProgram failed to load (type unknown - expected .glsl)");
		return false;
	}

	dispose();

	ShaderList shaders;
	auto disposeShaders = [&shaders]() -> void {
		for (auto& shader : shaders)
			shader.dispose();

		shaders.clear();
	};

	auto getShaderTypeFromString = [](const std::string& inType) -> ShaderStage::Type {
		if (inType == "vertex")
			return ShaderStage::Type::Vertex;

		if (inType == "fragment")
			return ShaderStage::Type::Fragment;

		if (inType == "geometry")
			return ShaderStage::Type::Geometry;

		if (inType == "compute")
			return ShaderStage::Type::Compute;

		return ShaderStage::Type::None;
	};

	auto source = ShaderStage::readFile(inFile);

	const std::string kToken("#pragma type :");
	auto pos = source.find(kToken, 0);
	while (pos != std::string::npos)
	{
		auto eol = source.find_first_of("\r\n", pos);
		if (eol == std::string::npos)
		{
			log_error("Failed to load material:", inFile);
			disposeShaders();
			return false;
		}

		auto beginType = pos + kToken.size() + 1;
		auto endType = eol - beginType;
		std::string type = source.substr(beginType, endType);

		ShaderStage::Type shaderType = getShaderTypeFromString(type);

		auto beginSource = source.find_first_not_of("\r\n", eol);
		pos = source.find(kToken, beginSource);
		auto endSource = pos - (beginSource == std::string::npos ? source.size() - 1 : beginSource);
		auto shaderSource = source.substr(beginSource, endSource);

		ShaderStage shader(inFile);
		bool result = shader.loadFromSource(shaderSource, shaderType);
		if (!result)
		{
			log_error("Failed to load material:", inFile);
			disposeShaders();
			return false;
		}

		shaders.emplace_back(std::move(shader));
	}

	bool result = loadFromShaders(shaders);
	disposeShaders();
	return result;
}

/*****************************************************************************/
bool ShaderProgram::loadFromShaders(const ShaderList& inShaders)
{
	m_id = glCreateProgram();

	{
		std::vector<ShaderStage::Type> shaderTypes;
		for (auto& shader : inShaders)
		{
			auto type = shader.type();
			for (ShaderStage::Type shaderType : shaderTypes)
			{
				if (type == shaderType)
				{
					log_error("ShaderStage program cannot contain duplicate shader types");
					dispose();
					return false;
				}
			}

			shaderTypes.emplace_back(shader.type());
			glCheck(glAttachShader(m_id, shader.id()));
		}
	}

	glCheck(glLinkProgram(m_id));

	i32 success = 0;
	glCheck(glGetProgramiv(m_id, GL_LINK_STATUS, &success));
	if (success == GL_FALSE)
	{
		std::array<char, 512> infoLog;
		glCheck(glGetProgramInfoLog(m_id, static_cast<GLsizei>(infoLog.size()), nullptr, infoLog.data()));

		dispose();

		log_error("ShaderStage program linking failed:", infoLog.data());
		return false;
	}

	return true;
}

/*****************************************************************************/
void ShaderProgram::bind() const
{
	if (kCurrentMaterial != this)
	{
		glCheck(glUseProgram(m_id));
		kCurrentMaterial = this;
	}
}

/*****************************************************************************/
void ShaderProgram::dispose()
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
void ShaderProgram::setFloat(const char* inName, f32 inValue)
{
	this->bind();
	i32 location = getUniformLocation(m_id, inName);
	glCheck(glUniform1f(location, inValue));
}

/*****************************************************************************/
void ShaderProgram::setVec2(const char* inName, f32 inX, f32 inY)
{
	this->bind();
	i32 location = getUniformLocation(m_id, inName);
	glCheck(glUniform2f(location, inX, inY));
}

/*****************************************************************************/
void ShaderProgram::setVec2(const char* inName, const Vec2f& inVec)
{
	this->bind();
	i32 location = getUniformLocation(m_id, inName);
	glCheck(glUniform2f(location, inVec.x, inVec.y));
}

/*****************************************************************************/
void ShaderProgram::setVec3(const char* inName, f32 inX, f32 inY, f32 inZ)
{
	this->bind();
	i32 location = getUniformLocation(m_id, inName);
	glCheck(glUniform3f(location, inX, inY, inZ));
}

/*****************************************************************************/
void ShaderProgram::setVec3(const char* inName, const Vec3f& inVec)
{
	this->bind();
	i32 location = getUniformLocation(m_id, inName);
	glCheck(glUniform3f(location, inVec.x, inVec.y, inVec.z));
}

/*****************************************************************************/
void ShaderProgram::setVec3(const char* inName, const Color& inColor)
{
	this->bind();
	i32 location = getUniformLocation(m_id, inName);
	f32 red = static_cast<f32>(inColor.r) / 255.0f;
	f32 green = static_cast<f32>(inColor.g) / 255.0f;
	f32 blue = static_cast<f32>(inColor.b) / 255.0f;
	glCheck(glUniform3f(location, red, green, blue));
}

/*****************************************************************************/
void ShaderProgram::setVec4(const char* inName, f32 inX, f32 inY, f32 inZ, f32 inW)
{
	this->bind();
	i32 location = getUniformLocation(m_id, inName);
	glCheck(glUniform4f(location, inX, inY, inZ, inW));
}

/*****************************************************************************/
void ShaderProgram::setVec4(const char* inName, const Color& inColor)
{
	this->bind();
	i32 location = getUniformLocation(m_id, inName);
	f32 red = static_cast<f32>(inColor.r) / 255.0f;
	f32 green = static_cast<f32>(inColor.g) / 255.0f;
	f32 blue = static_cast<f32>(inColor.b) / 255.0f;
	f32 alpha = static_cast<f32>(inColor.a) / 255.0f;
	glCheck(glUniform4f(location, red, green, blue, alpha));
}

/*****************************************************************************/
void ShaderProgram::setInt(const char* inName, i32 inValue)
{
	this->bind();
	i32 location = getUniformLocation(m_id, inName);
	glCheck(glUniform1i(location, inValue));
}

/*****************************************************************************/
void ShaderProgram::setTexture(const char* inName, const TextureBuffer& inTexture)
{
	return this->setInt(inName, inTexture.slot());
}

/*****************************************************************************/
void ShaderProgram::setMat3(const char* inName, const Mat3f& inValue)
{
	this->bind();
	i32 location = getUniformLocation(m_id, inName);
	glCheck(glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(inValue)));
}

/*****************************************************************************/
void ShaderProgram::setMat4(const char* inName, const Mat4f& inValue)
{
	this->bind();
	i32 location = getUniformLocation(m_id, inName);
	glCheck(glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(inValue)));
}

}
