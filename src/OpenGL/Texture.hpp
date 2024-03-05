#pragma once

#include "OpenGL/TextureBuffer.hpp"
#include "OpenGL/TextureKind.hpp"

namespace ogl
{
struct Texture
{
	TextureBuffer buffer;
	TextureKind kind = TextureKind::None;
	std::string path;
};
}
