#pragma once

#include "OpenGL/MeshAttribute.hpp"

namespace ogl
{
struct Material;
struct Mesh
{
	Mesh() = default;
	OGL_DEFAULT_COPY_MOVE(Mesh);
	virtual ~Mesh() = default;

	void dispose();

	Mesh& setGeometry(const std::vector<MeshAttribute>& inAttribs, std::vector<f32>&& inData);
	Mesh& setGeometry(const std::vector<MeshAttribute>& inAttribs, const std::vector<f32>& inData);

	void setMaterial(const Material& inMaterial);

	void draw() const;

private:
	void initialize();

	const Material* m_material = nullptr;

	std::vector<f32> m_data;

	u32 m_vbo = 0;
	// u32 m_ebo = 0;

	u32 m_vao = 0;
};
}
