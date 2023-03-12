#pragma once

#include "Libraries/OpenGL.hpp"

namespace ogl
{
struct GLBase
{
	GLBase() = default;
	GL_DEFAULT_COPY_MOVE(GLBase);
	virtual ~GLBase() = default;

	virtual void init() = 0;
	virtual void update() = 0;

	virtual i32 run() final;

protected:
	struct
	{
		std::string name;
		u32 width = 640;
		u32 height = 480;

	} m_data;
};
}

#define OGL_RUN_MAIN(x)       \
	int main()                \
	{                         \
		x program;            \
		return program.run(); \
	}
