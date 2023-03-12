#include "Core/Platform.hpp"

#include "Core/OpenGL.hpp"

#if defined(OG_WIN32)
	#include "Core/Win32/WindowsPlatform.hpp"
#endif

/*****************************************************************************/
void ogl::Platform::initialize(GLFWwindow* inWindow)
{
#if defined(OG_WIN32)
	ogl::WindowsPlatform platform;
	platform.initialize(inWindow);
#else
	UNUSED(inWindow);
#endif
}