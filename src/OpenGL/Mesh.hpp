#pragma once

#include "OpenGL/Attrib.hpp"

namespace ogl
{
struct Material;
struct Mesh
{
	Mesh() = default;
	OGL_DEFAULT_COPY_MOVE(Mesh);
	virtual ~Mesh() = default;

	void dispose();

	Mesh& setGeometry(const std::vector<Attrib>& inAttribs, std::vector<f32>&& inData);
	Mesh& setGeometry(const std::vector<Attrib>& inAttribs, const std::vector<f32>& inData);

	void setMaterial(const Material& inMaterial);

	void draw() const;

	u32 vbo() const; // temp?

private:
	void initialize();

	const Material* m_material = nullptr;

	std::vector<f32> m_data;

	u32 m_vbo = 0;
	// u32 m_ebo = 0;

	u32 m_vao = 0;
};
}
