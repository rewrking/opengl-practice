#pragma once

#include "Core/Image/Image.hpp"
#include "Libraries/Format.hpp"
#include "Libraries/GLM.hpp"
#include "Libraries/OpenGL.hpp"
#include "OpenGL/Camera.hpp"
#include "OpenGL/Material.hpp"
#include "OpenGL/Mesh.hpp"
#include "OpenGL/MeshOld.hpp"
#include "OpenGL/Model.hpp"
#include "OpenGL/TextureBuffer.hpp"

namespace ogl
{
struct EngineClock
{
	f32 deltaTime = 0.0f;
	f32 lastFrame = 0.0f;
};
struct ProgramBase
{
	struct Settings
	{
		std::string name = "Untitled";
		u32 width = 800;
		u32 height = 600;

		Settings() = default;
		Settings(const std::string& inName, u32 inWidth, u32 inHeight);
		explicit Settings(const std::string& inName);
		explicit Settings(u32 inWidth, u32 inHeight);
	};

	ProgramBase() = default;
	OGL_DEFAULT_COPY_MOVE(ProgramBase);
	virtual ~ProgramBase() = default;

	virtual void init() = 0;
	virtual void dispose() = 0;
	virtual void update() = 0;

	virtual Settings getSettings() const = 0;

	virtual bool processInput(GLFWwindow* window);

	virtual i32 run() final;

	virtual void onMouseMove(const f64 inX, const f64 inY);
	virtual void onMouseScroll(const f64 inX, const f64 inY);

	bool keyHeld(const i32 inKey) const;
	bool keyPressed(const i32 inKey) const;
	bool mouseButtonHeld(const i32 inKey) const;
	void processCameraControls(Camera& inCamera);

	void updateMouse();
	void clearContext();
	void useDepthBuffer();
	void setClearColor(const i32 inR, const i32 inG, const i32 inB);
	Color getColor(const i32 inR, const i32 inG, const i32 inB, const i32 inA = 255) const;

protected:
	EngineClock Clock;

	u32 m_width = 0;
	u32 m_height = 0;

	bool m_mouseInView = true;

	Camera& camera() noexcept;
	const Camera& camera() const noexcept;
	void setCameraEnabled(const bool inValue);

	void setWireframe(const bool inValue) const;

	Mat4f getProjectionMatrix() const;
	Mat4f getViewMatrix() const;

private:
	void setFullscreen(const bool inValue);

	void initializeLogger();

	Camera m_camera = Camera(Vec3f{ 1.25f, 1.0f, 4.0f });
	Vec2f m_lastMousePosition{ 0.0f, 0.0f };

	GLFWwindow* m_window = nullptr;

	struct
	{
		i32 x = 0;
		i32 y = 0;
		i32 width = 0;
		i32 height = 0;
		bool fullscreen = false;
	} m_windowProps;

	bool m_usingDepthBuffer = false;
	bool m_cameraEnabled = false;
	mutable bool m_wireframe = false;
	mutable bool m_keyPressed = false;
};
}
