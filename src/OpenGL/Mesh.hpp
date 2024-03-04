#pragma once

#include "OpenGL/GLM.hpp"
#include "OpenGL/Material.hpp"
#include "OpenGL/Texture.hpp"
#include "OpenGL/Vertex.hpp"

namespace ogl
{
struct Mesh
{
	using VertexList = std::vector<Vertex>;
	using IndexList = std::vector<u32>;
	using TextureList = std::vector<Texture>;

	VertexList vertices;
	IndexList indices;
	TextureList textures;

	Mesh(const VertexList& inVertices, const IndexList& inIndices, const TextureList& inTextures);
	void draw(Material& material);

private:
	u32 m_vao = 0;
	u32 m_vbo = 0;
	u32 m_ebo = 0;

	void setupMesh() noexcept;
};
}
