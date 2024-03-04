#pragma once

#include "OpenGL/Attrib.hpp"

namespace ogl
{
struct Material;
struct AbstractMesh
{
	AbstractMesh() = default;
	OGL_DEFAULT_COPY_MOVE(AbstractMesh);
	virtual ~AbstractMesh() = default;

	void dispose();

	AbstractMesh& setGeometry(const std::vector<Attrib>& inAttribs, std::vector<f32>&& inData);
	AbstractMesh& setGeometry(const std::vector<Attrib>& inAttribs, const std::vector<f32>& inData);

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
