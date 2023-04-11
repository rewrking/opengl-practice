#pragma once

#include "OpenGL/MeshAttribute.hpp"

namespace ogl
{
struct Mesh
{
	Mesh() = default;

	void dispose();

	Mesh& setGeometry(const std::vector<MeshAttribute>& inAttribs, std::vector<f32>&& inData);
	Mesh& setGeometry(const std::vector<MeshAttribute>& inAttribs, const std::vector<f32>& inData);

private:
	friend struct ShaderProgram;

	void initialize();
	void draw() const;

	std::vector<f32> m_data;

	u32 m_vbo = 0;
	// u32 m_ebo = 0;

	u32 m_vao = 0;
};
}
