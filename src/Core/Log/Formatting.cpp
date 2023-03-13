#include "Core/Log/Formatting.hpp"

#include "Core/Format.hpp"
#include "Core/Log/TerminalColor.hpp"

namespace ogl
{
/*****************************************************************************/
std::string fixedPrecision(const u8 inSize, const f64 inValue)
{
	std::stringstream stream;
	stream << std::fixed << std::setprecision(inSize) << inValue;
	return stream.str();
}

/*****************************************************************************/
std::string fixedPrecision(const u8 inSize, const f32 inValue)
{
	std::stringstream stream;
	stream << std::fixed << std::setprecision(inSize) << inValue;
	return stream.str();
}

/*****************************************************************************/
std::string outputGray(const std::string& inString)
{
	auto gray = Output::getAnsiStyle(TerminalColor::BrightBlack);
	auto reset = Output::getAnsiStyle(TerminalColor::Reset);

	return fmt::format("{}{}{}", gray, inString, reset);
}

/*****************************************************************************/
std::string outputBullet(const std::string& inString)
{
	auto gray = Output::getAnsiStyle(TerminalColor::BrightBlack);
	auto reset = Output::getAnsiStyle(TerminalColor::Reset);

	return fmt::format("{}‣ {}{}", gray, reset, inString);
}
}
