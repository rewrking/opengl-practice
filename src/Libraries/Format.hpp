#pragma once

#ifndef FMT_HEADER_ONLY
	#define FMT_HEADER_ONLY
#endif

#if defined(OGL_MSVC)
	#pragma warning(push)
	#pragma warning(disable : 4189)
	#pragma warning(disable : 4702)
#else
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wpedantic"
	#pragma GCC diagnostic ignored "-Wunused-parameter"
	#pragma GCC diagnostic ignored "-Wtype-limits"
	#pragma GCC diagnostic ignored "-Wshadow"
	#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"

	#if !defined(OGL_CLANG)
		#pragma GCC diagnostic ignored "-Wstringop-overflow"
		#pragma GCC diagnostic ignored "-Warray-bounds"
		#if __GNUC__ > 12
			#pragma GCC diagnostic ignored "-Wdangling-reference"
		#endif
	#endif
#endif

#include <fmt/color.h>
#include <fmt/core.h>
#include <fmt/format-inl.h>
#include <fmt/format.h>

#if defined(OGL_MSVC)
	#pragma warning(pop)
#else
	#pragma GCC diagnostic pop
#endif

#define FMT_ARG(x) fmt::arg(#x, x)
