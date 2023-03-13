#pragma once

/*
	MIT License

	Copyright (c) 2022 Andrew King

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/

/*
	"Terminal Bud"

	At the moment, this just normalizes the terminal output on Windows to use
	the UTF-8 code page and sets ENABLE_VIRTUAL_TERMINAL_PROCESSING.

	- Enables ansi escape sequences in command prompt/powershell

	Might build this out more later, who knows!
*/

#if defined(_WIN32)
	#ifndef UNICODE
		#define UNICODE
	#endif

	#ifndef _UNICODE
		#define _UNICODE
	#endif

	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>

	#if !defined(ENABLE_VIRTUAL_TERMINAL_PROCESSING)
		#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
	#endif
#endif

namespace termbud
{
namespace priv
{
#if defined(_WIN32)
static struct
{
	time_t comspec_last_write = -1;
	DWORD console_mode = 0;
	UINT console_code_page = 0;
	UINT console_output_code_page = 0;
} state;

struct stat statBuffer;
#endif
}

inline bool initialize()
{
#if defined(_WIN32)
	priv::state.console_output_code_page = GetConsoleOutputCP();
	priv::state.console_code_page = GetConsoleCP();

	auto ansi_colors_supported_in_com_spec = []() -> bool {
		auto get_environment_variable = [](const char* name) {
			const char* result = std::getenv(name);
			if (result != nullptr)
				return std::string(result);

			return std::string();
		};

		auto get_last_write_time = [](const std::string& file) -> time_t {
			if (!file.empty() && stat(file.c_str(), &priv::statBuffer) == 0)
				return priv::statBuffer.st_mtime;
			else
				return 0;
		};

		if (priv::state.comspec_last_write == -1)
		{
			priv::state.comspec_last_write = get_last_write_time(get_environment_variable("COMSPEC"));
			if (priv::state.comspec_last_write > 0)
				priv::state.comspec_last_write *= 1000;
		}

		// Note: ANSI terminal colors were added somewhere between Windows 10 build 10240 and 10586
		//   So we approximate based on the date of the first release of Windows 10 (build 10240, July 29, 2015)
		return priv::state.comspec_last_write > 1438128000000;
	};

	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hOut != INVALID_HANDLE_VALUE)
	{
		GetConsoleMode(hOut, &priv::state.console_mode);

		if (ansi_colors_supported_in_com_spec())
		{
			DWORD dwMode = 0;
			if (GetConsoleMode(hOut, &dwMode))
			{
				dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
				SetConsoleMode(hOut, dwMode);
			}
		}
	}

	if (SetConsoleOutputCP(CP_UTF8) == FALSE)
		return false;

	if (SetConsoleCP(CP_UTF8) == FALSE)
		return false;

#endif
	return true;
}

inline void dispose()
{
#if defined(_WIN32)
	if (priv::state.console_output_code_page != 0)
		SetConsoleOutputCP(priv::state.console_output_code_page);

	if (priv::state.console_code_page != 0)
		SetConsoleCP(priv::state.console_code_page);

	if (priv::state.console_mode != 0)
	{
		HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
		if (hOut != INVALID_HANDLE_VALUE)
		{
			SetConsoleMode(hOut, priv::state.console_mode);
		}
	}
#endif
}
}