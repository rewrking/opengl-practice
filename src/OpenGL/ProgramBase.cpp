#include "OpenGL/ProgramBase.hpp"

#include <array>

#include "Core/Log/LogManager.hpp"
#include "Core/Platform.hpp"

namespace ogl
{
namespace
{
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	UNUSED(window);
	glCheck(glViewport(0, 0, width, height));
}
}

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
ShaderProgram ProgramBase::loadShaderProgram(const StringList& inShaderFiles) const
{
	ShaderProgram shaderProgram;

	bool result = shaderProgram.load(inShaderFiles);
	if (!result)
	{
		throw std::runtime_error(std::string("Failed to load shader program!"));
	}

	return shaderProgram;
}

/*****************************************************************************/
bool ProgramBase::processInput(GLFWwindow* window)
{
	bool shouldClose = glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS;
	if (shouldClose)
	{
		glfwSetWindowShouldClose(window, true);
	}

	return !shouldClose;
}

/*****************************************************************************/
i32 ProgramBase::run()
{
	initializeLogger();

	GLFWwindow* window = nullptr;

	// Initialize the library
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);

	Settings settings = getSettings();

	auto glfwVersion = glfwGetVersionString();
	log_info("GLFW", glfwVersion);

	{
#if defined(OGL_MACOS)
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
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
		log_fatal("Failed to create window");
		glfwTerminate();
		return -1;
	}

	Platform::initialize(window);

	// Make the window's context current
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	int version = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	if (version == 0)
	{
		log_fatal("Failed to initialize GLAD");
		glfwTerminate();
		return -1;
	}

	log_info("-", glGetString(GL_RENDERER));
	log_info("-", glGetString(GL_VENDOR));
	log_info("-", glGetString(GL_VERSION));

	{
		i32 nrAttributes;
		glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
		log_info("- Max vertex attributes:", nrAttributes);
	}

	// glCheck(glViewport(0, 0, settings.width, settings.height));

	try
	{
		this->init();

		// Loop until the user closes the window
		while (!glfwWindowShouldClose(window))
		{
			if (!processInput(window))
				break;

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
		log_fatal("Exception thrown:", err.what());
	}

	try
	{
		this->dispose();
	}
	catch (const std::exception& err)
	{
		log_fatal("Exception thrown:", err.what());
	}

	glfwTerminate();
	glfwDestroyWindow(window);
	window = nullptr;

	LogManager::dispose();

	return 0;
};

/*****************************************************************************/
void ProgramBase::initializeLogger()
{
	LoggerSettings logSettings;
	logSettings.printKey = false;
	logSettings.outputFile = false;

#if defined(OGL_DEBUG)
	logSettings.outputStdout = true;
	logSettings.logLevel = LogLevel::Trace;
	logSettings.filename = "ogl-debug.log";
#else
	logSettings.outputStdout = false;
	logSettings.logLevel = LogLevel::Fatal;
#endif

	LogManager::initialize(logSettings);

	log_decor(std::string(80, '='));
}
}