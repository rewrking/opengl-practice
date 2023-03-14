#include "OpenGL/BufferAttribList.hpp"

namespace ogl
{
/*****************************************************************************/
BufferAttribList::BufferAttribList(const std::vector<i32>& inAttribSizes)
{
	i32 position = 0;
	size = 0;
	for (auto attribSize : inAttribSizes)
	{
		attribs.emplace_back(BufferAttrib{ position, size, attribSize });
		size += attribSize;
		position++;
	}
}
}
