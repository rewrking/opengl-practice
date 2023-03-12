#include "GLBase.hpp"

#include <array>

#include "Platform/Platform.hpp"

namespace ogl
{
// might be mac-only
using ProcAddressFn = void* (*)(const char*);

/*****************************************************************************/
i32 GLBase::run()
{
	GLFWwindow* window = nullptr;

	// Initialize the library
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);

#if defined(OGL_MACOS)
	const std::array<char, 2> versions[] = { { 4, 1 }, { 3, 3 }, { 3, 2 }, { 2, 1 } };
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);

	for (auto ver : versions)
	{
		if (ver[0] < 3)
		{
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, false);
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, ver[0]);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, ver[1]);
		window = glfwCreateWindow(m_data.width, m_data.height, m_data.name.data(), nullptr, nullptr);

		if (window)
			break;
	}

#elif defined(OGL_LINUX)
	glfwWindowHint(GLFW_VISIBLE, false);
	window = glfwCreateWindow(m_data.width, m_data.height, m_data.name.data(), nullptr, nullptr);
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

			window = glfwCreateWindow(m_data.width, m_data.height, m_data.name.data(), nullptr, nullptr);
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
	window = glfwCreateWindow(m_data.width, m_data.height, m_data.name.data(), nullptr, nullptr);
#endif

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
		printf("Failed to initialize OpenGL context\n");
		return -1;
	}

	std::cout << glGetString(GL_RENDERER) << "\n";
	std::cout << glGetString(GL_VENDOR) << "\n";
	std::cout << glGetString(GL_VERSION) << std::endl;

	this->init();

	// Loop until the user closes the window
	while (!glfwWindowShouldClose(window))
	{
		// Render here
		this->update();

		// Swap front and back buffers
		glfwSwapBuffers(window);

		// Poll for and process events
		glfwPollEvents();
	}

	glfwTerminate();
	window = nullptr;

	return 0;
};
}