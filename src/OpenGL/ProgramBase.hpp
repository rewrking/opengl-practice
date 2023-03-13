#pragma once

#include "Core/Format.hpp"
#include "OpenGL/GLM.hpp"
#include "OpenGL/OpenGL.hpp"
#include "OpenGL/ShaderProgram.hpp"

namespace ogl
{
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

	virtual ShaderProgram loadShaderProgram(const StringList& inShaderFiles) const final;

	virtual bool processInput(GLFWwindow* window);

	virtual i32 run() final;

private:
	void initializeLogger();
};
}

#define OGL_RUN_MAIN(x)       \
	int main()                \
	{                         \
		x program;            \
		return program.run(); \
	}

#define BUFFER_OFFSET(offset) ((void*)(offset))
