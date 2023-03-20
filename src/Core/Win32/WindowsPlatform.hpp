#pragma once

#include <array>

#include "Core/Win32/WindowsInclude.hpp"

struct GLFWwindow;

namespace ogl
{
struct WindowsPlatform
{
	WindowsPlatform();

	void initialize(GLFWwindow* inHandle);

private:
	float getScreenScalingFactor();

	PBYTE getIconDirectory(const int inResourceId);
	HICON getIconFromIconDirectory(PBYTE inIconDirectory, const std::uint32_t inSize);

	float m_screenScalingFactor = 0.0f;
};
}
