#include "OpenGL/Model.hpp"

#include "Core/Image/Image.hpp"
#include "Libraries/Assimp.hpp"
#include "Libraries/StbImage.hpp"

namespace ogl
{
/*****************************************************************************/
void Model::draw(Material& material) const
{
	for (auto& mesh : m_meshes)
		mesh.draw(material);
}

/*****************************************************************************/
bool Model::loaded() const noexcept
{
	return m_loaded;
}

/*****************************************************************************/
bool Model::load(const char* inPath)
{
	auto path = Image::getImagePath(inPath);

	Assimp::Importer import;
	auto scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	if (scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || scene->mRootNode == nullptr)
	{
		log_error("Error loading model:", import.GetErrorString());
		return false;
	}

	std::string basePath(inPath);
	m_directory = basePath.substr(0, basePath.find_last_of('/'));

	if (!processNode(*scene->mRootNode, *scene))
	{
		log_error("Error processing mesh nodes for:", path);
		return false;
	}

	m_loaded = true;
	return true;
}

/*****************************************************************************/
void Model::dispose()
{
	for (auto& mesh : m_meshes)
		mesh.dispose();

	m_meshes.clear();

	for (auto& texture : m_texturesLoaded)
	{
		texture.buffer.dispose();
	}

	m_texturesLoaded.clear();
	m_directory.clear();

	m_loaded = false;
}

/*****************************************************************************/
bool Model::processNode(aiNode& node, const aiScene& inScene)
{
	for (u32 i = 0; i < node.mNumMeshes; ++i)
	{
		auto mesh = inScene.mMeshes[node.mMeshes[i]];
		m_meshes.emplace_back(processMesh(*mesh, inScene));
	}

	for (u32 i = 0; i < node.mNumChildren; ++i)
	{
		if (!processNode(*node.mChildren[i], inScene))
			return false;
	}

	return true;
}

/*****************************************************************************/
Mesh Model::processMesh(aiMesh& mesh, const aiScene& inScene)
{
	Mesh out;

	for (u32 i = 0; i < mesh.mNumVertices; ++i)
	{
		out.vertices.emplace_back(makeVertex(mesh, i));
	}

	// process indices
	for (u32 i = 0; i < mesh.mNumFaces; ++i)
	{
		auto face = mesh.mFaces[i];

		for (u32 j = 0; j < face.mNumIndices; ++j)
			out.indices.emplace_back(face.mIndices[j]);
	}

	// process material
	if (mesh.mMaterialIndex >= 0)
	{
		auto material = inScene.mMaterials[mesh.mMaterialIndex];

		auto diffuseMaps = loadMaterialTextures(*material, aiTextureType_DIFFUSE, TextureKind::Diffuse);
		out.textures.insert(out.textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		auto specularMaps = loadMaterialTextures(*material, aiTextureType_SPECULAR, TextureKind::Specular);
		out.textures.insert(out.textures.end(), specularMaps.begin(), specularMaps.end());
	}

	if (!out.load())
	{
		log_error("Failed to load mesh");
	}

	return out;
}

/*****************************************************************************/
Vertex3D Model::makeVertex(aiMesh& mesh, u32 index)
{
	Vertex3D vertex;

	{
		auto& thisVertex = mesh.mVertices[index];
		vertex.position.x = static_cast<f32>(thisVertex.x);
		vertex.position.y = static_cast<f32>(thisVertex.y);
		vertex.position.z = static_cast<f32>(thisVertex.z);
	}

	if (mesh.HasNormals())
	{
		auto& thisNormal = mesh.mVertices[index];
		vertex.normal.x = static_cast<f32>(thisNormal.x);
		vertex.normal.y = static_cast<f32>(thisNormal.y);
		vertex.normal.z = static_cast<f32>(thisNormal.z);
	}

	if (mesh.mTextureCoords[0])
	{
		auto& thisNormal = mesh.mTextureCoords[0][index];

		// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't
		// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
		vertex.texCoords.x = static_cast<f32>(thisNormal.x);
		vertex.texCoords.y = static_cast<f32>(thisNormal.y);

		// vertex.tangent.x = mesh.mTangents[i].x;
		// vertex.tangent.y = mesh.mTangents[i].y;
		// vertex.tangent.z = mesh.mTangents[i].z;

		// vertex.bitangent.x = mesh.mBitangents[i].x;
		// vertex.bitangent.y = mesh.mBitangents[i].y;
		// vertex.bitangent.z = mesh.mBitangents[i].z;
	}
	else
	{
		vertex.texCoords.x = 0.0f;
		vertex.texCoords.y = 0.0f;
	}

	return vertex;
}

/*****************************************************************************/
Mesh::TextureList Model::loadMaterialTextures(aiMaterial& mat, i32 type, const TextureKind inType)
{
	Mesh::TextureList textures;

	auto textureType = static_cast<aiTextureType>(type);
	u32 textureCount = mat.GetTextureCount(textureType);
	for (u32 i = 0; i < textureCount; ++i)
	{
		aiString str;
		auto ret = mat.GetTexture(textureType, i, &str, nullptr, nullptr, nullptr, nullptr, nullptr);
		if (ret != aiReturn_SUCCESS)
			continue;

		bool skip = false;
		for (auto& texture : m_texturesLoaded)
		{
			if (::strcmp(texture.path.data(), str.C_Str()) == 0)
			{
				textures.emplace_back(&texture);
				skip = true;
				break;
			}
		}

		if (!skip)
		{
			m_texturesLoaded.emplace_back(makeTexture(str.C_Str(), inType));
			textures.emplace_back(&m_texturesLoaded.back());
		}
	}

	return textures;
}

/*****************************************************************************/
Texture Model::makeTexture(const char* path, const TextureKind inType) const
{
	Texture texture;

	texture.buffer = makeTextureFromFile(path, m_directory);
	texture.kind = inType;
	texture.path = std::string(path);

	return texture;
}

/*****************************************************************************/
TextureBuffer Model::makeTextureFromFile(const char* path, const std::string& directory) const
{
	stbi_set_flip_vertically_on_load(true);

	auto filename = fmt::format("{}/{}", directory, path);
	TextureBuffer buffer;
	TextureSettings settings;
	settings.smooth = true;

	if (!buffer.load(filename.c_str(), settings))
	{
		log_error("Failed to load:", filename);
	}
	else
	{
		log_info("Loaded texture:", filename);
	}

	return buffer;
}

}