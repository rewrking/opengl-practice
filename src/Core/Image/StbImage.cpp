#if defined(OGL_GCC) || defined(OGL_CLANG)
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wdouble-promotion"
	#pragma GCC diagnostic ignored "-Wcast-align"
	#pragma GCC diagnostic ignored "-Wmissing-declarations"
	#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#elif defined(OGL_MSVC)
	#pragma warning(push)
	#pragma warning(disable : 4996)
#endif

#if defined(__GNUC__) && !defined(NDEBUG) && defined(__OPTIMIZE__)
	#define OGL_UNDEF_OPTIMIZE
	#undef __OPTIMIZE__
#endif

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <stb/stb_image_resize.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

#if defined(OGL_UNDEF_OPTIMIZE)
	#undef OGL_UNDEF_OPTIMIZE
	#define __OPTIMIZE__
#endif

#if defined(OGL_GCC) || defined(OGL_CLANG)
	#pragma GCC diagnostic pop
#elif defined(OGL_MSVC)
	#pragma warning(pop)
#endif
