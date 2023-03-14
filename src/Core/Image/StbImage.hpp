#pragma once

#if defined(OGL_GCC) || defined(OGL_CLANG)
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wdouble-promotion"
	#pragma GCC diagnostic ignored "-Wcast-align"
	#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
	#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#elif defined(OGL_MSVC)
	#pragma warning(push)
	#pragma warning(disable : 4996)
#endif

#include <stb/stb_image.h>
#include <stb/stb_image_resize.h>
#include <stb/stb_image_write.h>

#if defined(OGL_GCC) || defined(OGL_CLANG)
	#pragma GCC diagnostic pop
#elif defined(OGL_MSVC)
	#pragma warning(pop)
#endif
