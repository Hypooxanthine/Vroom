#pragma once

#include "Vroom/Core/Platform.h"
#include "Vroom/Core/Log.h"
#include <iostream>

#ifdef VR_PLATFORM_WINDOWS
	#define VR_DEBUGBREAK __debugbreak()
#elif defined(VR_PLATFORM_LINUX)
	#include <signal.h>
	#define VR_DEBUGBREAK raise(SIGTRAP)
#endif

#define VR_ASSERT(check) if(!(check)) VR_DEBUGBREAK;
#define VR_ASSERT_MSG(check, ...) if(!(check)) { LOG_ERROR(std::string("Assertion failed : ") + __VA_ARGS__); VR_DEBUGBREAK; }