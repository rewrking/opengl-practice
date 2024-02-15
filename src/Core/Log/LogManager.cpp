#include "Core/Log/LogManager.hpp"

#include "Core/Libraries/Format.hpp"
#include "Core/Libraries/SpdLog.hpp"
#include "Core/Log/TerminalColor.hpp"

#if defined(OGL_MSVC)
	#pragma warning(push)
	#pragma warning(disable : 4996)
#endif

#include "Core/Log/TerminalBud.hpp"

namespace ogl
{
namespace
{
static bool loggerInitialized = false;

spdlog::level::level_enum getLogLevel(const LogLevel inLevel)
{
	switch (inLevel)
	{
		case LogLevel::Info:
			return spdlog::level::info;
		case LogLevel::Debug:
			return spdlog::level::debug;
		case LogLevel::Warning:
			return spdlog::level::warn;
		case LogLevel::Error:
			return spdlog::level::err;
		case LogLevel::Fatal:
			return spdlog::level::critical;
		default:
			return spdlog::level::trace;
	}
}
}

struct AnsiFormatter : spdlog::custom_flag_formatter
{
	void format(const spdlog::details::log_msg& msg, const std::tm&, spdlog::memory_buf_t& dest) override
	{
		const auto reset = Output::getAnsiStyle(TerminalColor::Reset);
		std::string output;
		if (msg.level == spdlog::level::info)
		{
			const auto color = Output::getAnsiStyle(TerminalColor::BrightBlue);
			output = fmt::format("{}[INFO]{}", color, reset);
		}
		else if (msg.level == spdlog::level::warn)
		{
			const auto color = Output::getAnsiStyle(TerminalColor::BrightYellow);
			output = fmt::format("{}[WARN]{}", color, reset);
		}
		else if (msg.level == spdlog::level::err)
		{
			const auto color = Output::getAnsiStyle(TerminalColor::BrightRed);
			output = fmt::format("{}[ERRR]{}", color, reset);
		}
		else if (msg.level == spdlog::level::critical)
		{
			const auto color = Output::getAnsiStyle(TerminalColor::RedInverted);
			output = fmt::format("{}[FATL]{}", color, reset);
		}
		else if (msg.level == spdlog::level::debug)
		{
			const auto color = Output::getAnsiStyle(TerminalColor::BrightGreen);
			output = fmt::format("{}[DEBG]{}", color, reset);
		}
		else
		{
			const auto color = Output::getAnsiStyle(TerminalColor::BrightBlack);
			output = fmt::format("{}[ .. ]{}", color, reset);
		}

		// if (msg.level != spdlog::level::trace)
		// 	output += ' ';

		dest.append(output.data(), output.data() + output.size());
	}

	std::unique_ptr<custom_flag_formatter> clone() const override
	{
		return spdlog::details::make_unique<AnsiFormatter>();
	}
};

struct PlainTextFormatter : spdlog::custom_flag_formatter
{
	void format(const spdlog::details::log_msg& msg, const std::tm&, spdlog::memory_buf_t& dest) override
	{
		std::string output;
		if (msg.level == spdlog::level::info)
			output = "[INFO]";
		else if (msg.level == spdlog::level::warn)
			output = "[WARN]";
		else if (msg.level == spdlog::level::err)
			output = "[ERRR]";
		else if (msg.level == spdlog::level::critical)
			output = "[FATL]";
		else if (msg.level == spdlog::level::debug)
			output = "[DEBG]";
		else
			output = "[ .. ]";

		// if (msg.level != spdlog::level::trace)
		// 	output += ' ';

		dest.append(output.data(), output.data() + output.size());
	}

	std::unique_ptr<custom_flag_formatter> clone() const override
	{
		return spdlog::details::make_unique<PlainTextFormatter>();
	}
};

/*****************************************************************************/
void LogManager::initialize(const LoggerSettings& inSettings)
{
	if (loggerInitialized)
		return;

	std::vector<spdlog::sink_ptr> sinks;

	if (inSettings.outputStdout)
	{
		auto sink = std::make_shared<spdlog::sinks::stdout_sink_mt>();
		auto formatter = std::make_unique<spdlog::pattern_formatter>();

		const auto boldBlack = Output::getAnsiStyle(TerminalColor::BrightBlack);
		const auto reset = Output::getAnsiStyle(TerminalColor::Reset);
		formatter->add_flag<AnsiFormatter>('*')
			.set_pattern(fmt::format("%^{}%H:%M:%S.%e %*{}:{} %v%$", boldBlack, boldBlack, reset));

		sink->set_formatter(std::move(formatter));
		sinks.emplace_back(sink);
	}

	if (inSettings.outputFile && !inSettings.filename.empty())
	{
		auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(inSettings.filename);
		auto formatter = std::make_unique<spdlog::pattern_formatter>();

		formatter->add_flag<PlainTextFormatter>('*')
			.set_pattern("%Y-%m-%d %^%H:%M:%S.%e %*: %v%$");

		sink->set_formatter(std::move(formatter));
		sinks.emplace_back(sink);
	}

	if (inSettings.outputStdout || inSettings.outputFile)
	{
		termbud::initialize();

		auto logger = std::make_shared<spdlog::logger>(OGL_DEFAULT_LOGGER_NAME, sinks.begin(), sinks.end());
		logger->set_level(getLogLevel(inSettings.logLevel));
		logger->flush_on(spdlog::level::trace);

		spdlog::register_logger(logger);

		if (inSettings.printKey)
		{
			log_decor("Decor");
			log_trace("Trace");
			log_debug("Debug");
			log_info("Info");
			log_warning("Warning");
			log_error("Error");
			log_fatal("Fatal");
		}
	}

	loggerInitialized = true;
}

/*****************************************************************************/
void LogManager::dispose() noexcept
{
	loggerInitialized = false;

	auto logger = spdlog::get(OGL_DEFAULT_LOGGER_NAME);
	if (logger != nullptr)
	{
		logger->flush();
		spdlog::shutdown();
		termbud::dispose();
	}
}

/*****************************************************************************/
bool LogManager::hasInitialized() noexcept
{
	return loggerInitialized;
}
}

#if defined(OGL_MSVC)
	#pragma warning(pop)
#endif
