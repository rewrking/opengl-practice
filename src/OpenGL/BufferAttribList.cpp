#include "OpenGL/BufferAttribList.hpp"

namespace ogl
{
/*****************************************************************************/
BufferAttribList::BufferAttribList(const std::vector<i32>& inAttribs)
{
	i32 position = 0;
	size = 0;
	for (i32 attribSize : inAttribs)
	{
		attribs.emplace_back(BufferAttrib{ position, size, attribSize });
		size += attribSize;
		position++;
	}
}

/*****************************************************************************/
BufferAttribList::BufferAttribList(const std::vector<MeshAttribute>& inAttribs)
{
	i32 position = 0;
	size = 0;
	for (auto attribSize : inAttribs)
	{
		attribs.emplace_back(BufferAttrib{ position, size, static_cast<i32>(attribSize) });
		size += attribSize;
		position++;
	}
}
}
