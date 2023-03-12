#include "ProgramBase.hpp"

#include <array>

#include "Core/Platform.hpp"

namespace ogl
{
// might be mac-only
using ProcAddressFn = void* (*)(const char*);

/*****************************************************************************/
ProgramBase::Settings::Settings(const std::string& inName, u32 inWidth, u32 inHeight) :
	name(inName),
	width(inWidth),
	height(inHeight)
{
}

/*****************************************************************************/
ProgramBase::Settings::Settings(const std::string& inName) :
	name(inName)
{
}

/*****************************************************************************/
ProgramBase::Settings::Settings(u32 inWidth, u32 inHeight) :
	width(inWidth),
	height(inHeight)
{
}

/*****************************************************************************/
std::string ProgramBase::getShader(const char* inPath) const
{
	return std::string("content/fx/") + inPath;
}

/*****************************************************************************/
GLuint ProgramBase::loadShaders(const std::vector<ShaderInfo>& inShaders) const
{
	UNUSED(inShaders);
	// throw std::runtime_error("Error loading shaders");
	return 0;
}

/*****************************************************************************/
void ProgramBase::processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

/*****************************************************************************/
i32 ProgramBase::run()
{
	GLFWwindow* window = nullptr;

	// Initialize the library
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);

	Settings settings = getSettings();

	{
#if defined(OGL_MACOS)
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
		window = glfwCreateWindow(settings.width, settings.height, settings.name.data(), nullptr, nullptr);
#elif defined(OGL_LINUX)
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
		window = glfwCreateWindow(settings.width, settings.height, settings.name.data(), nullptr, nullptr);
		if (window)
		{
			glfwMakeContextCurrent(window);
			std::string versionStr = glGetString(GL_VERSION);
			if (versionStr[0] < '4' && versionStr.contains("Mesa"))
			{
				glfwDestroyWindow(window);
				window = nullptr;

				// force 3.2 context
				glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
				glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

				window = glfwCreateWindow(settings.width, settings.height, settings.name.data(), nullptr, nullptr);
				if (window)
				{
					glfwMakeContextCurrent(window);
				}
				else
				{
					glfwTerminate();
					return -1;
				}
			}
			glfwShowWindow(window);
		}

#else
		// Create a windowed mode window and its OpenGL context
		window = glfwCreateWindow(settings.width, settings.height, settings.name.data(), nullptr, nullptr);
#endif
	}

	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	Platform::initialize(window);

	// Make the window's context current
	glfwMakeContextCurrent(window);

	int version = gladLoadGLLoader((ProcAddressFn)glfwGetProcAddress);
	if (version == 0)
	{
		std::cout << "Failed to initialize OpenGL context\n";
		return -1;
	}

	std::cout << glGetString(GL_RENDERER) << "\n";
	std::cout << glGetString(GL_VENDOR) << "\n";
	std::cout << glGetString(GL_VERSION) << std::endl;

	glCheck(glViewport(0, 0, settings.width, settings.height));

	try
	{
		this->init();

		// Loop until the user closes the window
		while (!glfwWindowShouldClose(window))
		{
			processInput(window);

			// Render here
			this->update();

			// Swap front and back buffers
			glfwSwapBuffers(window);

			// Poll for and process events
			glfwPollEvents();
		}
	}
	catch (const std::exception& err)
	{
		std::cerr << err.what() << '\n';
	}

	try
	{
		this->dispose();
	}
	catch (const std::exception& err)
	{
		std::cerr << err.what() << '\n';
	}

	glfwTerminate();
	glfwDestroyWindow(window);
	window = nullptr;

	return 0;
};
}