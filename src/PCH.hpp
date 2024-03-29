#ifndef PRECOMPILED_HEADER_HPP
#define PRECOMPILED_HEADER_HPP

#ifndef _DEBUG
	#ifndef NDEBUG
		#define NDEBUG
	#endif
#endif // _DEBUG

// Windows
#ifdef _WIN32
	#ifndef UNICODE
		#define UNICODE
	#endif

	#ifndef _UNICODE
		#define _UNICODE
	#endif
#endif // _WIN32

#include "Core/Defines.hpp"
#include "Core/Macros.hpp"

#include <algorithm>
#include <array>
#include <cstdio>
#include <deque>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include <functional>

#include "Libraries/Filesystem.hpp"
#include "Libraries/Format.hpp"

#include "Core/Log/Logger.hpp"
#include "Core/Types.hpp"
#include "Core/UnusedMacro.hpp"

#endif // PRECOMPILED_HEADER_HPP
