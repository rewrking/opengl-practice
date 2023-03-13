#include "Core/Platform.hpp"

#include "OpenGL/OpenGL.hpp"

#if defined(OGL_WIN32)
	#include "Core/Win32/WindowsPlatform.hpp"
#endif

/*****************************************************************************/
void ogl::Platform::initialize(GLFWwindow* inWindow)
{
#if defined(OGL_WIN32)
	ogl::WindowsPlatform platform;
	platform.initialize(inWindow);
#else
	UNUSED(inWindow);
#endif
}