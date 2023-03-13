#pragma once

#include "Core/Log/Formatting.hpp"
#include "Core/Log/LogLevel.hpp"
#include "Core/Macros.hpp"

#ifdef log
	#undef log
#endif

namespace ogl
{
namespace priv
{
template <typename T>
concept loggable_type = std::is_arithmetic_v<T> || std::is_pointer_v<T>;

namespace Logger
{
void _log(const LogLevel inLevel, const std::string& inString);
void _logClassMethod(const std::string& inFile, const std::string& inFunction);

template <typename Arg>
inline std::string logArgument(Arg&& arg) noexcept;

template <typename... Args>
inline void log(const LogLevel inLevel, Args&&... args);
}
}
}

#include "Core/Log/Logger.inl"

#if defined(OGL_ENABLE_LOGGING)
	#define log_trace(...) ogl::priv::Logger::log(ogl::LogLevel::Trace, __VA_ARGS__)
	#define log_decor(...) ogl::priv::Logger::log(ogl::LogLevel::Decor, __VA_ARGS__)
	#define log_info(...) ogl::priv::Logger::log(ogl::LogLevel::Info, __VA_ARGS__)
	#define log_warning(...) ogl::priv::Logger::log(ogl::LogLevel::Warning, __VA_ARGS__)
	#define log_error(...) ogl::priv::Logger::log(ogl::LogLevel::Error, __VA_ARGS__)
	#define log_fatal(...) ogl::priv::Logger::log(ogl::LogLevel::Fatal, __VA_ARGS__)
	#define log_debug(...) ogl::priv::Logger::log(ogl::LogLevel::Debug, __VA_ARGS__)
	#define log_separator() ogl::priv::Logger::log(ogl::LogLevel::Decor, std::string(80, '-'))
	#define log_class_method() ogl::priv::_logClassMethod(__FILE__, __FUNCTION__)
#else
	#define log_trace(...) UNUSED(__VA_ARGS__)
	#define log_decor(...) UNUSED(__VA_ARGS__)
	#define log_info(...) UNUSED(__VA_ARGS__)
	#define log_warning(...) UNUSED(__VA_ARGS__)
	#define log_error(...) UNUSED(__VA_ARGS__)
	#define log_fatal(...) UNUSED(__VA_ARGS__)
	#define log_debug(...) UNUSED(__VA_ARGS__)
	#define log_separator()
	#define log_class_method()
#endif
