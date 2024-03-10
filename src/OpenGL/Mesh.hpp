#pragma once

#include "Core/DefinesFeatures.hpp"
#include "Libraries/GLM.hpp"
#include "OpenGL/Material.hpp"
#include "OpenGL/Texture.hpp"
#include "OpenGL/Vertex3D.hpp"

namespace ogl
{
struct Mesh
{
	using IndexType = u32;
	using VertexType = Vertex3D;

	using VertexList = std::vector<VertexType>;
	using IndexList = std::vector<IndexType>;
	using TextureList = std::vector<Texture*>;

	Mesh() = default;

	bool load();
	void dispose();

	void draw(Material& inMaterial) const;

	VertexList vertices;
#if OGL_USE_INDICES
	IndexList indices;
#endif
	TextureList textures;

private:
	void setupMesh();

	u32 m_vao = 0;
	u32 m_vbo = 0;
#if OGL_USE_INDICES
	u32 m_ebo = 0;
#endif
};
}
