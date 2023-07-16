#include "OpenGL/ProgramBase.hpp"

namespace ogl
{
struct Program final : ProgramBase
{
	virtual Settings getSettings() const final
	{
		return Settings("00: Hello Window");
	}

	virtual void init() final
	{
		// Set the background color (Cornflower Blue - 100,149,237)
		setClearColor(100, 149, 237);
	}

	virtual void update() final
	{
		clearContext();
	}

	virtual void dispose() final
	{
	}
};
}

OGL_RUN_MAIN(ogl::Program);