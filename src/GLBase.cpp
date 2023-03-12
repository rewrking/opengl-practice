#include "GLBase.hpp"

#include "Platform/Platform.hpp"

namespace ogl
{
/*****************************************************************************/
i32 GLBase::run()
{
	GLFWwindow* window = nullptr;

	// Initialize the library
	if (!glfwInit())
		return -1;

	// Create a windowed mode window and its OpenGL context
	window = glfwCreateWindow(m_data.width, m_data.height, m_data.name.data(), nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	Platform::initialize(window);

	// Make the window's context current
	glfwMakeContextCurrent(window);

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
	return 0;
};
}