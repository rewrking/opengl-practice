#include "OpenGL/ProgramBase.hpp"

#include <array>
#include <thread>

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
	if (keyHeld(GLFW_KEY_ESCAPE))
	{
		glfwSetWindowShouldClose(window, true);
		return false;
	}

	if (m_cameraEnabled)
	{
		processCameraControls(m_camera);
	}

	return true;
}

/*****************************************************************************/
i32 ProgramBase::run()
{
	initializeLogger();

	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);

	Settings settings = getSettings();
	m_windowProps.width = static_cast<i32>(settings.width);
	m_windowProps.height = static_cast<i32>(settings.height);
	m_windowProps.fullscreen = false;

	auto glfwVersion = glfwGetVersionString();
	log_info("GLFW", glfwVersion);

	{
#if defined(OGL_MACOS)
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
		m_window = glfwCreateWindow(settings.width, settings.height, settings.name.data(), nullptr, nullptr);
#elif defined(OGL_LINUX)
		m_window = glfwCreateWindow(settings.width, settings.height, settings.name.data(), nullptr, nullptr);

#else
		m_window = glfwCreateWindow(settings.width, settings.height, settings.name.data(), nullptr, nullptr);
#endif
	}

	if (!m_window)
	{
		log_fatal("Failed to create window");
		glfwTerminate();
		return -1;
	}

	m_width = settings.width;
	m_height = settings.height;

	Platform::initialize(m_window);

	glfwMakeContextCurrent(m_window);

	glfwSetWindowUserPointer(m_window, this);

	if (glfwRawMouseMotionSupported())
		glfwSetInputMode(m_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

	glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* win, i32 width, i32 height) {
		if (win)
		{
			glCheck(glViewport(0, 0, width, height));
		}
	});
	glfwSetWindowSizeCallback(m_window, [](GLFWwindow* win, i32 width, i32 height) {
		if (win)
		{
			auto self = static_cast<ProgramBase*>(glfwGetWindowUserPointer(win));
			self->m_width = static_cast<u32>(width);
			self->m_height = static_cast<u32>(height);
		}
	});

	glfwSetCursorPosCallback(m_window, [](GLFWwindow* win, f64 xpos, f64 ypos) {
		if (win)
		{
			auto self = static_cast<ProgramBase*>(glfwGetWindowUserPointer(win));
			// if (self->m_mouseInView)
			{
				self->onMouseMove(xpos, ypos);
			}
		}
	});

	glfwSetCursorEnterCallback(m_window, [](GLFWwindow* win, i32 entered) {
		if (win)
		{
			auto self = static_cast<ProgramBase*>(glfwGetWindowUserPointer(win));
			self->m_mouseInView = entered == GLFW_TRUE;
		}
	});

	glfwSetScrollCallback(m_window, [](GLFWwindow* win, f64 xoffset, f64 yoffset) {
		if (win)
		{
			auto self = static_cast<ProgramBase*>(glfwGetWindowUserPointer(win));
			self->onMouseScroll(xoffset, yoffset);
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
		m_lastMousePosition.x = static_cast<f32>(m_width / 2);
		m_lastMousePosition.y = static_cast<f32>(m_height / 2);

		this->init();

		updateMouse();

		// Loop until the user closes the window
		while (!glfwWindowShouldClose(m_window))
		{
			f64 currentFrame = glfwGetTime();
			Clock.deltaTime = static_cast<f32>(currentFrame) - Clock.lastFrame;
			Clock.lastFrame = static_cast<f32>(currentFrame);

			if (!processInput(m_window))
				break;

			if (m_cameraEnabled)
			{
				m_camera.update(Clock.deltaTime);
			}

			this->update();

			glfwSwapBuffers(m_window);
			glfwPollEvents();

			// We just want to slow rendering down "enough" until this is done properly
			std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<u64>((1.0 / 65.0) * 1000.0)));
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
	glfwDestroyWindow(m_window);
	m_window = nullptr;

	LogManager::dispose();

	return 0;
};

/*****************************************************************************/
void ProgramBase::updateMouse()
{
	f64 xpos = 0.0;
	f64 ypos = 0.0;
	glfwGetCursorPos(m_window, &xpos, &ypos);

	onMouseMove(xpos, ypos);
}

/*****************************************************************************/
void ProgramBase::onMouseMove(const f64 inX, const f64 inY)
{
	f32 xpos = static_cast<f32>(inX);
	f32 ypos = static_cast<f32>(inY);

	f32 xoffset = xpos - m_lastMousePosition.x;
	f32 yoffset = m_lastMousePosition.y - ypos;

	m_lastMousePosition.x = xpos;
	m_lastMousePosition.y = ypos;

	if (m_cameraEnabled)
	{
		m_camera.processMouseMovement(xoffset, yoffset);
	}
}

/*****************************************************************************/
void ProgramBase::onMouseScroll(const f64 inX, const f64 inY)
{
	f32 xpos = static_cast<f32>(inX);
	f32 ypos = static_cast<f32>(inY);

	if (m_cameraEnabled)
	{
		UNUSED(xpos);
		m_camera.processMouseScroll(ypos);
	}
}

/*****************************************************************************/
bool ProgramBase::keyHeld(const i32 inKey) const
{
	return glfwGetKey(m_window, inKey) == GLFW_PRESS;
}

/*****************************************************************************/
bool ProgramBase::keyPressed(const i32 inKey) const
{
	if (keyHeld(inKey))
	{
		if (!m_keyPressed)
		{
			m_keyPressed = true;
			return true;
		}
	}
	else
	{
		m_keyPressed = false;
	}

	return false;
}

/*****************************************************************************/
bool ProgramBase::mouseButtonHeld(const i32 inKey) const
{
	return glfwGetMouseButton(m_window, inKey) == GLFW_PRESS;
}

/*****************************************************************************/
void ProgramBase::processCameraControls(Camera& inCamera)
{
	if (mouseButtonHeld(GLFW_MOUSE_BUTTON_LEFT))
		inCamera.processMouseButton(MouseButton::Left);
	else if (mouseButtonHeld(GLFW_MOUSE_BUTTON_MIDDLE))
		inCamera.processMouseButton(MouseButton::Middle);
	else if (mouseButtonHeld(GLFW_MOUSE_BUTTON_RIGHT))
		inCamera.processMouseButton(MouseButton::Right);
	else
		inCamera.processMouseButton(MouseButton::None);

	inCamera.processKeyboard(CameraMovement::None);

	if (keyHeld(GLFW_KEY_UP) || keyHeld(GLFW_KEY_W))
		inCamera.processKeyboard(CameraMovement::Forward);
	else if (keyHeld(GLFW_KEY_DOWN) || keyHeld(GLFW_KEY_S))
		inCamera.processKeyboard(CameraMovement::Backward);

	if (keyHeld(GLFW_KEY_LEFT) || keyHeld(GLFW_KEY_A))
		inCamera.processKeyboard(CameraMovement::Left);
	else if (keyHeld(GLFW_KEY_RIGHT) || keyHeld(GLFW_KEY_D))
		inCamera.processKeyboard(CameraMovement::Right);

	if (keyPressed(GLFW_KEY_F4))
		setFullscreen(!m_windowProps.fullscreen);
}

/*****************************************************************************/
void ProgramBase::clearContext()
{
	i32 flags = GL_COLOR_BUFFER_BIT;

	if (m_usingDepthBuffer)
		flags |= GL_DEPTH_BUFFER_BIT;

	glCheck(glClear(flags));
}

/*****************************************************************************/
void ProgramBase::useDepthBuffer()
{
	glCheck(glEnable(GL_DEPTH_TEST));
	m_usingDepthBuffer = true;
}

/*****************************************************************************/
void ProgramBase::setClearColor(const i32 inR, const i32 inG, const i32 inB)
{
	glCheck(glClearColor(static_cast<f32>(inR) / 255.0f, static_cast<f32>(inG) / 255.0f, static_cast<f32>(inB) / 255.0f, 1.0f));
}

/*****************************************************************************/
Color ProgramBase::getColor(const i32 inR, const i32 inG, const i32 inB, const i32 inA) const
{
	return Color{
		static_cast<f32>(inR) / 255.0f,
		static_cast<f32>(inG) / 255.0f,
		static_cast<f32>(inB) / 255.0f,
		static_cast<f32>(inA) / 255.0f,
	};
}

/*****************************************************************************/
const Camera& ProgramBase::camera() const noexcept
{
	return m_camera;
}

/*****************************************************************************/
void ProgramBase::setCameraEnabled(const bool inValue)
{
	m_cameraEnabled = inValue;
}

/*****************************************************************************/
void ProgramBase::setWireframe(const bool inValue) const
{
	if (m_wireframe != inValue)
	{
		m_wireframe = inValue;

		// also, GL_POINT

		if (m_wireframe)
			glCheck(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
		else
			glCheck(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
	}
}

/*****************************************************************************/
void ProgramBase::setFullscreen(const bool inValue)
{
	if (!m_windowProps.fullscreen)
	{
		glfwGetWindowPos(m_window, &m_windowProps.x, &m_windowProps.y);
	}

	if (inValue)
	{
		GLFWmonitor* monitor = glfwGetWindowMonitor(m_window);
		if (monitor == nullptr)
			monitor = glfwGetPrimaryMonitor();

		if (monitor)
		{
			const GLFWvidmode* mode = glfwGetVideoMode(monitor);
			glfwSetWindowMonitor(m_window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
		}
	}
	else
	{
		i32 xpos = m_windowProps.x;
		i32 ypos = m_windowProps.y;
		i32 width = m_windowProps.width;
		i32 height = m_windowProps.height;
		glfwSetWindowMonitor(m_window, NULL, xpos, ypos, width, height, 0);
	}
	m_windowProps.fullscreen = inValue;
}

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