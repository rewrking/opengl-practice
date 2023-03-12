#pragma once

#include "Core/GLM.hpp"
#include "Core/OpenGL.hpp"
#include "Core/ShaderInfo.hpp"

namespace ogl
{

struct ProgramBase
{
	struct Settings
	{
		std::string name = "Untitled";
		u32 width = 640;
		u32 height = 480;

		Settings() = default;
		Settings(const std::string& inName, u32 inWidth, u32 inHeight);
		explicit Settings(const std::string& inName);
		explicit Settings(u32 inWidth, u32 inHeight);
	};

	ProgramBase() = default;
	GL_DEFAULT_COPY_MOVE(ProgramBase);
	virtual ~ProgramBase() = default;

	virtual void init() = 0;
	virtual void dispose() = 0;
	virtual void update() = 0;

	virtual Settings getSettings() const = 0;

	virtual std::string getShader(const char* inPath) const final;
	virtual GLuint loadShaders(const std::vector<ShaderInfo>& inShaders) const final;

	virtual i32 run() final;
};
}

#define OGL_RUN_MAIN(x)       \
	int main()                \
	{                         \
		x program;            \
		return program.run(); \
	}

#define BUFFER_OFFSET(offset) ((void*)(offset))
