#include "OpenGL/Model.hpp"

#include "Libraries/Assimp.hpp"
#include "Libraries/StbImage.hpp"

namespace ogl
{
/*****************************************************************************/
void Model::draw(Material& material)
{
	for (auto& mesh : m_meshes)
		mesh.draw(material);
}

/*****************************************************************************/
bool Model::loadModel(const std::string& inPath)
{
	Assimp::Importer import;
	auto scene = import.ReadFile(inPath, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || scene->mRootNode == nullptr)
	{
		log_error("Error loading model:", import.GetErrorString());
		return false;
	}

	m_directory = inPath.substr(0, inPath.find_last_of('/'));

	if (!processNode(*scene->mRootNode, *scene))
	{
		log_error("Error processing mesh nodes for:", inPath);
		return false;
	}

	return true;
}

/*****************************************************************************/
bool Model::processNode(aiNode& node, const aiScene& inScene)
{
	for (u32 i = 0; i < node.mNumMeshes; i++)
	{
		auto mesh = inScene.mMeshes[node.mMeshes[i]];
		m_meshes.emplace_back(processMesh(*mesh, inScene));
	}

	for (u32 i = 0; i < node.mNumChildren; i++)
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
		aiFace face = mesh.mFaces[i];

		for (u32 j = 0; j < face.mNumIndices; ++j)
			out.indices.emplace_back(face.mIndices[j]);
	}

	// process material
	if (mesh.mMaterialIndex >= 0)
	{
		auto material = inScene.mMaterials[mesh.mMaterialIndex];

		auto diffuseMaps = loadMaterialTextures(*material, aiTextureType_DIFFUSE, "texture_diffuse");
		out.textures.insert(out.textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		auto specularMaps = loadMaterialTextures(*material, aiTextureType_SPECULAR, "texture_specular");
		out.textures.insert(out.textures.end(), specularMaps.begin(), specularMaps.end());
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
Mesh::TextureList Model::loadMaterialTextures(aiMaterial& mat, i32 type, std::string typeName)
{
	Mesh::TextureList textures;

	auto textureType = static_cast<aiTextureType>(type);
	u32 textureCount = mat.GetTextureCount(textureType);
	for (u32 i = 0; i < textureCount; ++i)
	{
		aiString str;
		mat.GetTexture(textureType, i, &str);
		bool skip = false;
		for (auto& texture : m_texturesLoaded)
		{
			if (::strcmp(texture.path.data(), str.C_Str()) == 0)
			{
				textures.emplace_back(texture);
				skip = true;
				break;
			}
		}
		if (!skip)
		{
			Texture texture;
			texture.id = makeTextureFromFile(str.C_Str(), m_directory);
			texture.type = typeName;
			texture.path = std::string(str.C_Str());
			textures.emplace_back(texture);
		}
	}

	return textures;
}

/*****************************************************************************/
u32 Model::makeTextureFromFile(const char* path, const std::string& directory) const
{
	auto filename = fmt::format("{}/{}", directory, path);

	u32 textureID = 0;
	glGenTextures(1, &textureID);

	i32 width = 0;
	i32 height = 0;
	i32 nrComponents = 0;
	auto data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;
		else
			return textureID;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

}
