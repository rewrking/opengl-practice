#include "Core/Log/Logger.hpp"

#include "Core/Log/LogManager.hpp"
#include "Core/Log/SpdLog.hpp"
#include "Core/Log/TerminalColor.hpp"

namespace ogl::priv
{
/*****************************************************************************/
// Takes __FILE__ and __FUNCTION__ and parses __FILE__ to base name
//
void Logger::_logClassMethod(const std::string& inFile, const std::string& inFunction)
{
	size_t lastSlash = inFile.find_last_of("/\\") + 1;
	size_t period = inFile.find_last_of(".");

	auto str = inFile.substr(lastSlash, period - lastSlash) + "::" + inFunction + "()";

	auto logger = spdlog::get(OGL_DEFAULT_LOGGER_NAME);
	if (logger != nullptr)
	{
		logger->debug(str);
	}
}

/*****************************************************************************/
void Logger::_log(const LogLevel inLevel, const std::string& inString)
{
	auto logger = spdlog::get(OGL_DEFAULT_LOGGER_NAME);
	if (logger != nullptr)
	{
		if (inLevel == LogLevel::Trace)
			logger->trace(inString);
		else if (inLevel == LogLevel::Info)
			logger->info(inString);
		else if (inLevel == LogLevel::Warning)
			logger->warn(inString);
		else if (inLevel == LogLevel::Error)
			logger->error(inString);
		else if (inLevel == LogLevel::Fatal)
			logger->critical(inString);
		else if (inLevel == LogLevel::Debug)
			logger->debug(inString);
		else if (inLevel == LogLevel::Decor)
		{
			const auto gray = Output::getAnsiStyle(TerminalColor::BrightBlack);
			const auto reset = Output::getAnsiStyle(TerminalColor::Reset);
			logger->trace(fmt::format("{}{}{}", gray, inString, reset));
		}
	}
	else if (!LogManager::hasInitialized())
	{
		if (inLevel == LogLevel::Error || inLevel == LogLevel::Fatal)
			std::cerr << inString << std::endl;
		else
			std::cout << inString << std::endl;
	}
}
}
