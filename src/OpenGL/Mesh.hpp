#pragma once

#include "Libraries/GLM.hpp"
#include "OpenGL/Material.hpp"
#include "OpenGL/Texture.hpp"
#include "OpenGL/Vertex3D.hpp"

namespace ogl
{
struct Mesh
{
	using VertexList = std::vector<Vertex3D>;
	using IndexList = std::vector<u32>;
	using TextureList = std::vector<Texture>;

	Mesh() = default;

	bool load();
	void draw(Material& material);

	VertexList vertices;
	IndexList indices;
	TextureList textures;

private:
	u32 m_vao = 0;
	u32 m_vbo = 0;
	u32 m_ebo = 0;

	void setupMesh();
};
}
