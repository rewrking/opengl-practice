#include "OpenGL/Material.hpp"

namespace ogl
{
/*****************************************************************************/
template <base_of<Material> T>
[[nodiscard]] T Material::make(const StringList& inShaderFiles)
{
	Material ret;
	ret.loadFromFiles(inShaderFiles);
	return ret;
}

}
