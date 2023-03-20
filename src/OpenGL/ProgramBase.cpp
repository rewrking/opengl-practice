#include "OpenGL/ProgramBase.hpp"

#include <array>

#include "Core/Log/LogManager.hpp"
#include "Core/Platform.hpp"

namespace ogl
{
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
		window = glfwCreateWindow(settings.width, settings.height, settings.name.data(), nullptr, nullptr);

#else
		window = glfwCreateWindow(settings.width, settings.height, settings.name.data(), nullptr, nullptr);
#endif
	}

	if (!window)
	{
		log_fatal("Failed to create window");
		glfwTerminate();
		return -1;
	}

	m_width = settings.width;
	m_height = settings.height;

	Platform::initialize(window);

	glfwSetWindowUserPointer(window, this);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, [](GLFWwindow* win, int width, int height) {
		if (win)
		{
			glCheck(glViewport(0, 0, width, height));
		}
	});
	glfwSetWindowSizeCallback(window, [](GLFWwindow* win, int width, int height) {
		if (win)
		{
			auto self = static_cast<ProgramBase*>(glfwGetWindowUserPointer(win));
			self->m_width = static_cast<u32>(width);
			self->m_height = static_cast<u32>(height);
		}
	});

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
		glCheck(glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes));
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

			this->update();

			glfwSwapBuffers(window);
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