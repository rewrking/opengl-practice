#pragma once

#define OGL_DEFAULT_LOGGER_NAME "oglog"

#if defined(OGL_MSVC)
// #pragma warning(push)
#else
	#pragma GCC diagnostic push

	#if !defined(OGL_CLANG)
		#pragma GCC diagnostic ignored "-Wstringop-overflow"
		#pragma GCC diagnostic ignored "-Warray-bounds"
		#if __GNUC__ > 12
			#pragma GCC diagnostic ignored "-Wdangling-reference"
		#endif
	#endif
#endif

#if !defined(SPDLOG_FMT_EXTERNAL)
	#define SPDLOG_FMT_EXTERNAL
#endif

#include "Libraries/Format.hpp"

#include <spdlog/spdlog.h>

#include <spdlog/pattern_formatter.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_sinks.h>

#if defined(OGL_MSVC)
// #pragma warning(pop)
#else
	#pragma GCC diagnostic pop
#endif
