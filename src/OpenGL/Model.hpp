#pragma once

#include "OpenGL/Mesh.hpp"

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

	void draw(Material& material);

	bool loadModel(const std::string& inPath);

private:
	bool processNode(aiNode& node, const aiScene& inScene);
	Vertex3D makeVertex(aiMesh& mesh, u32 index);
	Mesh processMesh(aiMesh& mesh, const aiScene& inScene);
	Mesh::TextureList loadMaterialTextures(aiMaterial& mat, i32 type, std::string typeName);
	u32 makeTextureFromFile(const char* path, const std::string& directory) const;

	MeshList m_meshes;
	Mesh::TextureList m_texturesLoaded;
	std::string m_directory;
};
}
