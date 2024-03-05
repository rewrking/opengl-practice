#pragma once

#include "OpenGL/TextureSettings.hpp"

namespace ogl
{
struct Image;

struct TextureBuffer
{
	TextureBuffer() = default;

	bool loaded() const noexcept;

	bool load(const char* inPath, const TextureSettings& settings = TextureSettings());
	bool load(const Image& inImage, const TextureSettings& settings = TextureSettings());
	void dispose();

	void bind(const i32 inSlot = 0) const;

	i32 slot() const noexcept;

private:
	void assign(const i32 inSlot) const;

	static const TextureBuffer* kCurrentTexture;

	mutable i32 m_slot = -1;
	u32 m_texture = 0;
};
}
