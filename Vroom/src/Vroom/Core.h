#pragma once

#ifdef VR_PLATFORM_WINDOWS
	#ifdef VR_BUILD_DLL
		#define VROOM_API __declspec(dllexport)
	#else
		#define VROOM_API __declspec(dllimport)
	#endif
#else
	#error Vroom only supports Windows for now.
#endif