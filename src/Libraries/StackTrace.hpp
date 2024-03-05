#pragma once

#include "Libraries/WindowsApi.hpp"

#if defined(OGL_MSVC) && !defined(OGL_CLANG)
	#pragma warning(push)
	#pragma warning(disable : 4456)
#else
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wshadow"
	#pragma GCC diagnostic ignored "-Wunused-result"
	#pragma GCC diagnostic ignored "-Wunused-variable"
	#pragma GCC diagnostic ignored "-Wsign-compare"
#endif

#include <ust/ust.hpp>

#if defined(OGL_MSVC) && !defined(OGL_CLANG)
	#pragma warning(pop)
#else
	#pragma GCC diagnostic pop
#endif
