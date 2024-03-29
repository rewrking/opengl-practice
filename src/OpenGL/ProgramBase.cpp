#include "OpenGL/ProgramBase.hpp"

#include <array>
#include <thread>

#include "Core/Log/LogManager.hpp"
#include "Core/Platform.hpp"
#include "Core/System/ExceptionTranslator.hpp"
#include "Core/System/SignalHandler.hpp"

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
	i32 result = OGL_EXIT_SUCCESS;

	{
#if defined(OGL_MSVC)
		ExceptionTranslator exceptionTranslator;
#endif
		SignalHandler::initialize([this]() {
			this->dispose();

			glfwTerminate();
			glfwDestroyWindow(m_window);
			m_window = nullptr;
		});

		initializeLogger();

		if (!glfwInit())
			return OGL_EXIT_FAILURE;

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

		int version = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		if (version == 0)
		{
			log_fatal("Failed to initialize GLAD");
			glfwTerminate();
			return -1;
		}

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

		log_info("-", glGetString(GL_RENDERER));
		log_info("-", glGetString(GL_VENDOR));
		log_info("-", glGetString(GL_VERSION));

		{
			i32 nrAttributes;
			glCheck(glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes));
			log_info("- Max vertex attributes:", nrAttributes);
		}

		// glCheck(glViewport(0, 0, settings.width, settings.height));

		OGL_TRY
		{
			m_lastMousePosition.x = static_cast<f32>(m_width / 2);
			m_lastMousePosition.y = static_cast<f32>(m_height / 2);

			this->init();

			updateMouse();

			// Loop until the user closes the window
			while (!SignalHandler::signaled && !glfwWindowShouldClose(m_window))
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
				std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<u64>((1.0 / 125.0) * 1000.0)));
			}
		}
		OGL_CATCH(const std::exception& err)
		{
			log_fatal("Exception thrown:", err.what());
			result = OGL_EXIT_FAILURE;
		}

		OGL_TRY
		{
			this->dispose();
		}
		OGL_CATCH(const std::exception& err)
		{
			log_fatal("Exception thrown:", err.what());
			result = OGL_EXIT_FAILURE;
		}

		glfwMakeContextCurrent(nullptr);
		glfwDestroyWindow(m_window);
		glfwTerminate();
		m_window = nullptr;
	}

	LogManager::dispose();

	return result;
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
Camera& ProgramBase::camera() noexcept
{
	return m_camera;
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
Mat4f ProgramBase::getProjectionMatrix() const
{
	// distance from camera to clip front (must be greater than 0)
	constexpr f32 near = 0.1f;

	// distance from camera to clip rear
	constexpr f32 far = 50.0f;

	// return Mat4f(1.0f);
	return glm::perspective(m_camera.getFieldOfView(), static_cast<f32>(m_width) / static_cast<f32>(m_height), near, far);
}

/*****************************************************************************/
Mat4f ProgramBase::getViewMatrix() const
{
	// auto cameraPos = Vec3f{ 0.0f, 0.0f, -3.0f };
	// note that we're translating the scene in the reverse direction of where we want to move
	// m_view = Mat4f(1.0f);
	// m_view = glm::translate(Mat4f(1.0f), cameraPos);

	// constexpr f64 radius = 10.0;
	// f32 camX = static_cast<f32>(std::sin(glfwGetTime()) * radius);
	// f32 camZ = static_cast<f32>(std::cos(glfwGetTime()) * radius);
	// auto cameraPos = Vec3f{ camX, 0.0, camZ };

	return m_camera.getViewMatrix();
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