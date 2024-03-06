#include "Core/System/SignalHandler.hpp"

#include "Core/Log/LogManager.hpp"
#include "Core/Log/TerminalColor.hpp"
#include "Libraries/Filesystem.hpp"
#include "Libraries/Format.hpp"
#include "Libraries/StackTrace.hpp"
#include "Libraries/WindowsApi.hpp"

#include <stdio.h>
#include <stdlib.h>
#if defined(OGL_WIN32)
	#include <cstring>
#endif

namespace ogl
{
namespace
{
Function<void> sOnErrorCallback = nullptr;
}

void printStackTrace();
void printError(const char* inType, const char* inDescription, const bool inPrintStackTrace = true);

/*****************************************************************************/
void printStackTrace()
{
	auto cwd = fs::current_path().string();
	if (cwd.empty())
		return;

#if defined(OGL_WIN32)
	cwd[0] = static_cast<uchar>(::toupper(cwd[0]));
#endif

	const auto colorAt = Output::getAnsiStyle(TerminalColor::BrightMagenta);
	const auto gray = Output::getAnsiStyle(TerminalColor::BrightBlack);
	const auto redHighlight = Output::getAnsiStyle(TerminalColor::BrightMagentaInverted);
	const auto blue = Output::getAnsiStyle(TerminalColor::BrightBlue);
	const auto reset = Output::getAnsiStyle(TerminalColor::Reset);

	StringList ignoreList
	{
		"ogl::SignalHandler",
			"ogl::printStackTrace",
			"ogl::printError",
#if defined(OGL_WIN32)
			// noise from ExceptionTranslator
			"ogl::translatorFunction",
			"__NLG_Return2",
			"__CxxFrameHandler4",
			"__GSHandlerCheck_EH4",
			"__chkstk",
			"RtlRaiseException",
			"KiUserExceptionDispatcher",
#endif
	};

	auto startsWith = [](const StringList& inFind, const std::string& inString) {
		if (!inString.empty())
		{
			for (auto& item : inFind)
			{
				if (item.size() < inFind.size())
					continue;

				if (std::equal(item.begin(), item.end(), inString.begin()))
					return true;
			}
		}

		return false;
	};

	bool highlight = true;
	ust::StackTrace stacktrace = ust::generate();
	for (auto& entry : stacktrace.entries)
	{
		if (startsWith(ignoreList, entry.functionName))
			continue;

		LogLevel logLevel = LogLevel::Fatal;

		if (entry.functionName.size() > 100)
			entry.functionName = entry.functionName.substr(0, 100) + std::string("...");

#if defined(OGL_WIN32)
		entry.sourceFileName[0] = static_cast<uchar>(::toupper(entry.sourceFileName[0]));
#endif

		// note: entry.sourceFileName will be blank if there are no debugging symbols!
#if defined(OGL_MACOS)
		bool sourceCode = !entry.sourceFileName.empty();
		std::string sourceFile = entry.sourceFileName;
#else
		size_t pos = entry.sourceFileName.find(cwd);
		bool sourceCode = pos != std::string::npos;
		std::string sourceFile = sourceCode ? entry.sourceFileName.substr(pos + cwd.length() + 1) : entry.sourceFileName;
#endif

		// Note: breakpoints vary depending on compiler optimization level
		std::stringstream message;
		if (sourceCode)
		{
			// at TestScene::init() src/main/Scenes/TestScene.cpp:42
			if (highlight)
			{
				message << redHighlight << " at ";
				highlight = false;
			}
			else
			{
				message << colorAt << " at ";
				// logLevel = LogLevel::Error;
			}

			message << reset << ' ' << entry.functionName << ' ' << blue << sourceFile << ':' << entry.lineNumber << reset;
		}
		// OS dynamic libs, etc
		else if (entry.functionName.empty())
		{
			// at C:/Windows/System32/msvcrt.dll:0x7ff8f04e7c58
			// Skip these, because they're just noise (until they're not)
			// message << boldRed << "  at " << colorUnimportant << entry.binaryFileName << ':' << entry.address << reset;
		}
		// C++ runtime, libstdc++, libgcc, etc.
		else
		{
			// at mainCRTStartup
			message << colorAt << " at  " << gray << entry.functionName << ' ' << entry.lineNumber << reset;
			logLevel = LogLevel::Error;
		}

		priv::Logger::log(logLevel, message.str());
	}
}

/*****************************************************************************/
void printError(const char* inType, const char* inDescription, const bool inPrintStackTrace)
{
	const auto boldRed = Output::getAnsiStyle(TerminalColor::RedBold);
	const auto reset = Output::getAnsiStyle(TerminalColor::Reset);

	if (::strcmp(inDescription, "") != 0)
		priv::Logger::log(LogLevel::Fatal, fmt::format("{}{} - {}{}", boldRed, inType, inDescription, reset));
	else
		priv::Logger::log(LogLevel::Fatal, fmt::format("{}{}{}", boldRed, inType, reset));

	if (inPrintStackTrace)
		printStackTrace();
}

/*****************************************************************************/
bool SignalHandler::signaled = false;

/*****************************************************************************/
void SignalHandler::initialize(Function<void> inOnError)
{
	sOnErrorCallback = inOnError;

	std::signal(SIGABRT, SignalHandler::handler);
	std::signal(SIGFPE, SignalHandler::handler);
	std::signal(SIGILL, SignalHandler::handler);
	std::signal(SIGINT, SignalHandler::handler);
	std::signal(SIGSEGV, SignalHandler::handler);
	std::signal(SIGTERM, SignalHandler::handler);
}

/*****************************************************************************/
void SignalHandler::handler(const i32 inSignal)
{
	if (signaled)
		return;

	signaled = true;

	priv::Logger::log(LogLevel::Trace, "");

	bool exceptionThrown = std::current_exception() != nullptr;
	bool assertionFailure = false; // not rigged up

	const auto boldRed = Output::getAnsiStyle(TerminalColor::MagentaBold);
	const auto reset = Output::getAnsiStyle(TerminalColor::Reset);

	switch (inSignal)
	{
		case SIGABRT: {
			if (exceptionThrown)
				printError("SIGABRT", "Exception Thrown");
			else if (assertionFailure)
				printError("SIGABRT", "Assertion Failure");
			else
				printError("SIGABRT", "Abort");
			break;
		}

		case SIGFPE:
			printError("SIGFPE", "Floating Point Exception (such as divide by zero)");
			break;

		case SIGILL:
			printError("SIGILL", "Illegal Instruction");
			break;

		case SIGINT:
			printError("SIGINT", "Terminal Interrupt");
			break;

		case SIGSEGV:
			printError("SIGSEGV", "Segmentation Fault");
			break;

		case SIGTERM:
			printError("SIGTERM", "Termination Request");
			break;

		default:
			priv::Logger::log(ogl::LogLevel::Fatal, fmt::format("Unknown Signal: {}", inSignal));
			printStackTrace();
			break;
	}

	if (sOnErrorCallback != nullptr)
		sOnErrorCallback();

	LogManager::dispose();

	::_exit(OGL_EXIT_FAILURE);
}
}
