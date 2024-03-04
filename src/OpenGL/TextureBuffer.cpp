#include "OpenGL/TextureBuffer.hpp"

#include "Core/Image/Image.hpp"
#include "OpenGL/OpenGL.hpp"

namespace ogl
{
constexpr i32 getTextureSlot(const u32 inSlot)
{
	switch (inSlot)
	{
		case 0: return GL_TEXTURE0;
		case 1: return GL_TEXTURE1;
		case 2: return GL_TEXTURE2;
		case 3: return GL_TEXTURE3;
		case 4: return GL_TEXTURE4;
		case 5: return GL_TEXTURE5;
		case 6: return GL_TEXTURE6;
		case 7: return GL_TEXTURE7;
		case 8: return GL_TEXTURE8;
		case 9: return GL_TEXTURE9;
		case 10: return GL_TEXTURE10;
		case 11: return GL_TEXTURE11;
		case 12: return GL_TEXTURE12;
		case 13: return GL_TEXTURE13;
		case 14: return GL_TEXTURE14;
		case 15: return GL_TEXTURE15;
		case 16: return GL_TEXTURE16;
		case 17: return GL_TEXTURE17;
		case 18: return GL_TEXTURE18;
		case 19: return GL_TEXTURE19;
		case 20: return GL_TEXTURE20;
		case 21: return GL_TEXTURE21;
		case 22: return GL_TEXTURE22;
		case 23: return GL_TEXTURE23;
		case 24: return GL_TEXTURE24;
		case 25: return GL_TEXTURE25;
		case 26: return GL_TEXTURE26;
		case 27: return GL_TEXTURE27;
		case 28: return GL_TEXTURE28;
		case 29: return GL_TEXTURE29;
		case 30: return GL_TEXTURE30;
		case 31: return GL_TEXTURE31;
	}
	return GL_TEXTURE0;
}

/*****************************************************************************/
const TextureBuffer* TextureBuffer::kCurrentTexture = nullptr;

/*****************************************************************************/
bool TextureBuffer::load(const char* inPath)
{
	auto image = Image::make(inPath);
	return load(image);
}

/*****************************************************************************/
bool TextureBuffer::load(const Image& inImage)
{
	if (m_texture > 0)
		return false;

	glCheck(glGenTextures(1, &m_texture));
	glCheck(glBindTexture(GL_TEXTURE_2D, m_texture));

	/*
		GL_REPEAT
		GL_MIRRORED_REPEAT
		GL_CLAMP_TO_EDGE
		GL_CLAMP_TO_BORDER
	*/
	glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

	/*
		GL_NEAREST
		GL_LINEAR

	// control mipmaps levels
		GL_NEAREST_MIPMAP_NEAREST
		GL_LINEAR_MIPMAP_NEAREST
		GL_NEAREST_MIPMAP_LINEAR
		GL_LINEAR_MIPMAP_LINEAR
	*/
	glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

	auto format = inImage.channels == 3 ? GL_RGB : GL_RGBA;

	glCheck(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, static_cast<GLsizei>(inImage.width), static_cast<GLsizei>(inImage.height), 0, format, GL_UNSIGNED_BYTE, inImage.pixels.data()));
	glCheck(glGenerateMipmap(GL_TEXTURE_2D));

	return true;
}

/*****************************************************************************/
void TextureBuffer::dispose()
{
	if (kCurrentTexture == this)
		kCurrentTexture = nullptr;

	if (m_texture > 0)
	{
		glCheck(glDeleteTextures(1, &m_texture));
		m_texture = 0;
	}

	m_slot = -1;
}

/*****************************************************************************/
void TextureBuffer::bind(const i32 inSlot) const
{
	if (kCurrentTexture != this && m_texture > 0)
	{
		assign(inSlot);

		glCheck(glBindTexture(GL_TEXTURE_2D, m_texture));
		kCurrentTexture = this;
	}
}

/*****************************************************************************/
void TextureBuffer::assign(const i32 inSlot) const
{
	if (m_texture > 0)
	{
		m_slot = inSlot;
		glCheck(glActiveTexture(getTextureSlot(inSlot)));
	}
}

/*****************************************************************************/
i32 TextureBuffer::slot() const noexcept
{
	if (m_slot < 0)
	{
		log_debug("texture not assigned a slot:", m_slot);
		return 0;
	}

	return m_slot;
}
}
