#include "OpenGL/Material.hpp"

namespace ogl
{
/*****************************************************************************/
template <base_of<Material> T>
[[nodiscard]] T Material::make(const std::string& inFile)
{
	Material ret;
	ret.loadFromFile(inFile);
	return ret;
}

/*****************************************************************************/
template <base_of<Material> T>
[[nodiscard]] T Material::makeFromFiles(const StringList& inShaderFiles)
{
	Material ret;
	ret.loadFromFiles(inShaderFiles);
	return ret;
}
}
