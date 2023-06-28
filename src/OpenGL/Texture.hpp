#pragma once

namespace ogl
{
struct Image;

struct Texture
{
	Texture() = default;

	bool load(const char* inPath);
	bool load(const Image& inImage);
	void dispose();

	void bind(const i32 inSlot = 0) const;

	i32 slot() const noexcept;

private:
	void assign(const i32 inSlot) const;

	static const Texture* kCurrentTexture;

	mutable i32 m_slot = -1;
	u32 m_texture = 0;
};
}
