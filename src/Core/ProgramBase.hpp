#pragma once

#include "Core/GLM.hpp"
#include "Core/OpenGL.hpp"

namespace ogl
{
struct ProgramSettings
{
	std::string name;
	u32 width = 0;
	u32 height = 0;
};

struct ProgramBase
{
	ProgramBase() = default;
	GL_DEFAULT_COPY_MOVE(ProgramBase);
	virtual ~ProgramBase() = default;

	virtual void init() = 0;
	virtual void update() = 0;

	virtual ProgramSettings getSettings() const;

	virtual i32 run() final;
};
}

#define OGL_RUN_MAIN(x)       \
	int main()                \
	{                         \
		x program;            \
		return program.run(); \
	}
