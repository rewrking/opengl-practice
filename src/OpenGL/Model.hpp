#pragma once

#include "OpenGL/Mesh.hpp"
#include "OpenGL/TextureBuffer.hpp"
#include "OpenGL/TextureKind.hpp"

struct aiNode;
struct aiMesh;
struct aiScene;
struct aiMaterial;

namespace ogl
{
struct Model
{
	using MeshList = std::vector<Mesh>;

	Model() = default;

	void draw(Material& material) const;

	bool load(const char* inPath);
	void dispose();

private:
	using TextureList = std::vector<Texture>;

	bool processNode(aiNode& node, const aiScene& inScene);
	Vertex3D makeVertex(aiMesh& mesh, u32 index);
	Mesh processMesh(aiMesh& mesh, const aiScene& inScene);
	Mesh::TextureList loadMaterialTextures(aiMaterial& mat, i32 type, const TextureKind inType);
	Texture makeTexture(const char* path, const TextureKind inType) const;
	TextureBuffer makeTextureFromFile(const char* path, const std::string& directory) const;

	MeshList m_meshes;
	TextureList m_texturesLoaded;
	std::string m_directory;
};
}
