#include "Core/Log/TerminalColor.hpp"

#include "Core/Format.hpp"

namespace ogl
{
namespace
{
/*****************************************************************************/
constexpr char getEscapeChar()
{
	return '\x1b';
}
}

/*****************************************************************************/
std::string Output::getAnsiStyle(const TerminalColor inColor)
{
	if (inColor == TerminalColor::None)
		return std::string();

	// #if defined(OGL_WIN32)
	// 	bool isCmdPromptLike = Environment::isCommandPromptOrPowerShell();
	// 	if (isCmdPromptLike && !ansiColorsSupportedInComSpec())
	// 		return std::string();
	// #endif

	constexpr char esc = getEscapeChar();
	if (inColor == TerminalColor::Reset)
		return fmt::format("{}[0m", esc);

	using ColorType = std::underlying_type_t<TerminalColor>;
	ColorType color = static_cast<ColorType>(inColor);
	ColorType style = color / static_cast<ColorType>(100);
	if (color > 100)
		color -= (style * static_cast<ColorType>(100));

	// #if defined(OGL_WIN32)
	// if (isCmdPromptLike)
	// 	return fmt::format("{}[{}m{}[{}m", esc, style, esc, color);
	// #endif

	return fmt::format("{}[{};{}m", esc, style, color);
}
}