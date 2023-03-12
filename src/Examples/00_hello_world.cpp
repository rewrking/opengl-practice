#include "Core/ProgramBase.hpp"

namespace ogl
{
struct Program final : ProgramBase
{
	virtual Settings getSettings() const final
	{
		return Settings("00: Hello World");
	}

	virtual void init() final
	{
		// Set the background color (Cornflower Blue - 100,149,237)
		glCheck(glClearColor(100.0f / 255.0f, 149.0f / 255.0f, 237.0f / 255.0f, 1.0f));
	}

	virtual void update() final
	{
		glCheck(glClear(GL_COLOR_BUFFER_BIT));
	}

	virtual void dispose() final
	{
	}
};
}

OGL_RUN_MAIN(ogl::Program);