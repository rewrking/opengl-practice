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

	void assign(const u32 inSlot) const;
	void bind() const;

private:
	static const Texture* kCurrentTexture;

	u32 m_texture = 0;
};
}
