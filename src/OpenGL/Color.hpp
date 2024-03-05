#pragma once

namespace ogl
{
struct Color
{
	constexpr Color() = default;
	constexpr Color(const u8 inRed, const u8 inGreen, const u8 inBlue, const u8 inAlpha = 255);
	constexpr Color(const Color& inColor, const u8 inAlpha);

	static constexpr Color fromRGBA(const u32 inRGBA);
	static constexpr Color fromRGB(const i32 inRGB);

	constexpr u32 toInteger() const noexcept;

	constexpr bool operator==(const Color& rhs) const noexcept;
	constexpr bool operator!=(const Color& rhs) const noexcept;

	constexpr Color& operator+=(const Color& rhs) noexcept;
	constexpr Color& operator-=(const Color& rhs) noexcept;
	constexpr Color& operator*=(const Color& rhs) noexcept;

	constexpr Color operator+(const Color& rhs);
	constexpr Color operator-(const Color& rhs);
	constexpr Color operator*(const Color& rhs) noexcept;

	u8 r = 0;
	u8 g = 0;
	u8 b = 0;
	u8 a = 255;
};
}

#include "OpenGL/Color.inl"
