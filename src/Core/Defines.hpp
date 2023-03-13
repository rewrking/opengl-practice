#pragma once

#if defined(_DEBUG) || defined(DEBUG)
	#ifndef OGL_DEBUG
		#define OGL_DEBUG
	#endif
#endif

#if defined(__APPLE__)
	#ifndef OGL_MACOS
		#define OGL_MACOS
	#endif
#elif defined(__linux__)
	#ifndef OGL_LINUX
		#define OGL_LINUX
	#endif
	#if defined(__arm__)
		#ifndef OGL_LINUX_ARM
			#define OGL_LINUX_ARM
		#endif
	#endif
#elif defined(_WIN32)
	#ifndef OGL_WIN32
		#define OGL_WIN32
	#endif
#else
	// static_assert(false, "Unknown platform");
	#error "Unknown platform"
#endif

#if defined(__clang__)
	#define OGL_CLANG
#endif

#if defined(_MSC_VER)
	#define OGL_MSVC
#elif defined(__GNUC__)
	#define OGL_GCC
#endif

#if defined(OGL_WIN32)
	#define OGL_EXIT_SUCCESS EXIT_SUCCESS
	#define OGL_EXIT_FAILURE 1067
#else
	#define OGL_EXIT_SUCCESS EXIT_SUCCESS
	#define OGL_EXIT_FAILURE EXIT_FAILURE
#endif

#if defined(OGL_DEBUG)
	#define OGL_ENABLE_LOGGING
#endif
