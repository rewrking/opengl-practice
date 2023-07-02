#include "OpenGL/BufferAttribList.hpp"

namespace ogl
{
/*****************************************************************************/
BufferAttribList::BufferAttribList(const std::vector<Attrib>& inAttribs)
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
