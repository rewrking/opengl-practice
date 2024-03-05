#include "OpenGL/Color.hpp"

namespace ogl
{
/*****************************************************************************/
constexpr Color::Color(const u8 inRed, const u8 inGreen, const u8 inBlue, const u8 inAlpha) :
	r(inRed),
	g(inGreen),
	b(inBlue),
	a(inAlpha)
{
}

/*****************************************************************************/
constexpr Color::Color(const Color& inColor, const u8 inAlpha) :
	r(inColor.r),
	g(inColor.g),
	b(inColor.b),
	a(inAlpha)
{
}

/*****************************************************************************/
constexpr Color Color::fromRGBA(const u32 inRGBA)
{
	Color col;
	col.r = static_cast<u8>((inRGBA & 0xff000000) >> 24);
	col.g = static_cast<u8>((inRGBA & 0x00ff0000) >> 16);
	col.b = static_cast<u8>((inRGBA & 0x0000ff00) >> 8);
	col.a = static_cast<u8>((inRGBA & 0x000000ff) >> 0);
	return col;
}

/*****************************************************************************/
constexpr Color Color::fromRGB(const i32 inRGB)
{
	Color col;
	col.r = static_cast<u8>((inRGB & 0xff0000) >> 16);
	col.g = static_cast<u8>((inRGB & 0x00ff00) >> 8);
	col.b = static_cast<u8>((inRGB & 0x0000ff) >> 0);
	return col;
}

/*****************************************************************************/
constexpr u32 Color::toInteger() const noexcept
{
	return static_cast<u32>((r << 24) | (g << 16) | (b << 8) | a);
}

/*****************************************************************************/
constexpr bool Color::operator==(const Color& rhs) const noexcept
{
	return (r == rhs.r) && (g == rhs.g) && (b == rhs.b) && (a == rhs.a);
}

/*****************************************************************************/
constexpr bool Color::operator!=(const Color& rhs) const noexcept
{
	return !(*this == rhs);
}

/*****************************************************************************/
constexpr Color& Color::operator+=(const Color& rhs) noexcept
{
	r += rhs.r;
	g += rhs.g;
	b += rhs.b;
	a += rhs.a;

	return *this;
}

/*****************************************************************************/
constexpr Color& Color::operator-=(const Color& rhs) noexcept
{
	r -= rhs.r;
	g -= rhs.g;
	b -= rhs.b;
	a -= rhs.a;

	return *this;
}

/*****************************************************************************/
constexpr Color& Color::operator*=(const Color& rhs) noexcept
{
	r *= rhs.r;
	g *= rhs.g;
	b *= rhs.b;
	a *= rhs.a;

	return *this;
}

/*****************************************************************************/
constexpr Color Color::operator+(const Color& rhs)
{
	return Color(u8(std::min(i32(r) + rhs.r, 255)),
		u8(std::min(i32(g) + rhs.g, 255)),
		u8(std::min(i32(b) + rhs.b, 255)),
		u8(std::min(i32(a) + rhs.a, 255)));
}

/*****************************************************************************/
constexpr Color Color::operator-(const Color& rhs)
{
	return Color(u8(std::max(i32(r) - rhs.r, 0)),
		u8(std::max(i32(g) - rhs.g, 0)),
		u8(std::max(i32(b) - rhs.b, 0)),
		u8(std::max(i32(a) - rhs.a, 0)));
}

/*****************************************************************************/
constexpr Color Color::operator*(const Color& rhs) noexcept
{
	return Color(u8(i32(r) * rhs.r / 255),
		u8(i32(g) * rhs.g / 255),
		u8(i32(b) * rhs.b / 255),
		u8(i32(a) * rhs.a / 255));
}

}