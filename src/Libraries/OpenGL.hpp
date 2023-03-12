#pragma once

#if defined(OGL_MSVC) && defined(OGL_CLANG)
	#pragma clang diagnostic push
	#pragma clang diagnostic ignored "-Wlanguage-extension-token"
#endif

#include <glad/glad.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#if defined(OGL_MSVC) && defined(OGL_CLANG)
	#pragma clang diagnostic pop
#endif

namespace ogl
{
namespace priv
{
void doGLCheck(const char* inFile, u32 inLine, const char* inExpression);
}
}

#if defined(OGL_DEBUG)
	// clang-format off
	#define glCheck(expr) do { expr; ogl::priv::doGLCheck(__FILE__, __LINE__, #expr); } while (false)
// clang-format on
#else
	#define glCheck(expr) (expr)
#endif
