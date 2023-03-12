#include "Platform/Platform.hpp"

#include "Libraries/OpenGL.hpp"

#if defined(_WIN32)
	#include "Platform/Win32/WindowsPlatform.hpp"
#endif

/*****************************************************************************/
void ogl::Platform::initialize(GLFWwindow* inWindow)
{
#if defined(_WIN32)
	ogl::WindowsPlatform platform;
	platform.initialize(inWindow);
#else
	UNUSED(inWindow);
#endif
}