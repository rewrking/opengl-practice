#pragma once

#include "Core/Types.hpp"

namespace ogl
{
std::string fixedPrecision(const u8 inSize, const f64 inValue);
std::string fixedPrecision(const u8 inSize, const f32 inValue);

std::string outputGray(const std::string& inString);
std::string outputBullet(const std::string& inString);
}
