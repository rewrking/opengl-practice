#pragma once

#include "OpenGL/MeshAttribute.hpp"

namespace ogl
{
struct BufferAttrib
{
	i32 position;
	i32 offset;
	i32 size;
};
struct BufferAttribList
{
	std::vector<BufferAttrib> attribs;
	i32 size = 0;

	explicit BufferAttribList(const std::vector<i32>& inAttribs);
	explicit BufferAttribList(const std::vector<MeshAttribute>& inAttribs);
};
}
