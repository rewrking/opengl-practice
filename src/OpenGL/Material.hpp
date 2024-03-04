#pragma once

#include "Core/Concepts.hpp"
#include "OpenGL/GLM.hpp"
#include "OpenGL/OpenGL.hpp"
#include "OpenGL/Shader.hpp"

namespace ogl
{
struct MeshOld;
struct TextureBuffer;
struct Material
{
	Material() = default;
	OGL_DEFAULT_COPY_MOVE(Material);
	virtual ~Material() = default;

	bool loadFromFiles(const StringList& inShaderFiles);
	bool loadFromFile(const std::string& inFile);

	u32 id() const noexcept;
	bool valid() const noexcept;

	void bind() const;

	void dispose();

	void setFloat(const char* inName, f32 inValue);
	void setVec2(const char* inName, f32 inX, f32 inY);
	void setVec2(const char* inName, const Vec2f& inVec);
	void setVec3(const char* inName, f32 inX, f32 inY, f32 inZ);
	void setVec3(const char* inName, const Vec3f& inVec);
	void setVec4(const char* inName, f32 inX, f32 inY, f32 inZ, f32 inW);
	void setVec4(const char* inName, const Color& inColor);

	void setInt(const char* inName, i32 inValue);
	void setTexture(const char* inName, const TextureBuffer& inTexture);

	void setMat3(const char* inName, const Mat3f& inValue);
	void setMat4(const char* inName, const Mat4f& inValue);

protected:
	bool loadFromShaders(const ShaderList& inShaders);

	u32 m_id = 0;

private:
	static const Material* kCurrentMaterial;
};
}
