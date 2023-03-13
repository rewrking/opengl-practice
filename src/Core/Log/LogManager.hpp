#pragma once

#include "Core/Log/LogLevel.hpp"

namespace ogl
{
struct LoggerSettings
{
	std::string filename;
	LogLevel logLevel = LogLevel::Trace;
	bool outputFile = false;
	bool outputStdout = false;
	bool printKey = false;
};

struct LogManager
{
	LogManager() = delete;

	static void initialize(const LoggerSettings& inSettings);
	static void dispose() noexcept;

	static bool hasInitialized() noexcept;
};
}
