#pragma once

#include "Core/Concepts.hpp"
#include "OpenGL/GLM.hpp"
#include "OpenGL/OpenGL.hpp"
#include "OpenGL/Shader.hpp"

namespace ogl
{
struct Mesh;
struct Material
{
	Material() = default;
	OGL_DEFAULT_COPY_MOVE(Material);
	virtual ~Material() = default;

	template <base_of<Material> T = Material>
	[[nodiscard]] static T make(const StringList& inShaderFiles);

	u32 id() const noexcept;
	bool valid() const noexcept;

	void bind() const;

	void dispose();

	void setUniform1f(const char* inName, f32 inValue);
	void setUniform2f(const char* inName, f32 inX, f32 inY);
	void setUniform2f(const char* inName, const Vec2f& inVec);
	void setUniform3f(const char* inName, f32 inX, f32 inY, f32 inZ);
	void setUniform3f(const char* inName, const Vec3f& inVec);
	void setUniform4f(const char* inName, f32 inX, f32 inY, f32 inZ, f32 inW);
	void setUniform4f(const char* inName, const Color& inColor);

	void setUniform1i(const char* inName, i32 inValue);

	void setUniformMatrix4f(const char* inName, const Mat4f& inValue);

protected:
	bool loadFromFiles(const StringList& inShaderFiles);

	u32 m_id = 0;

private:
	static const Material* kCurrentMaterial;
};
}

#include "OpenGL/Material.inl"
