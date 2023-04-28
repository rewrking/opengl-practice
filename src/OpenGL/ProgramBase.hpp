#pragma once

#include "Core/Format.hpp"
#include "Core/Image/Image.hpp"
#include "OpenGL/Camera.hpp"
#include "OpenGL/GLM.hpp"
#include "OpenGL/Material.hpp"
#include "OpenGL/Mesh.hpp"
#include "OpenGL/OpenGL.hpp"
#include "OpenGL/Texture.hpp"

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

	bool keyPressed(const i32 inKey) const;
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

private:
	void initializeLogger();

	GLFWwindow* m_window = nullptr;

	bool m_usingDepthBuffer = false;
};
}

#define OGL_RUN_MAIN(x)       \
	int main()                \
	{                         \
		x program;            \
		return program.run(); \
	}

#define BUFFER_OFFSET(offset) ((void*)(offset))
