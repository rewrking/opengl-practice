#pragma once

#if defined(OGL_WIN32)
	#ifndef UNICODE
		#define UNICODE
	#endif

	#ifndef _UNICODE
		#define _UNICODE
	#endif

	#ifndef WIN32_ICON_MAIN
		#define WIN32_ICON_MAIN 2
	#endif

	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
	#include <winuser.h>

	#if defined(_MSC_VER)
		#pragma execution_character_set("utf-8")
	#endif
#endif
