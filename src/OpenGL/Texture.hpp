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

	void use(const u32 inSlot) const;

private:
	u32 m_texture = 0;
};
}
