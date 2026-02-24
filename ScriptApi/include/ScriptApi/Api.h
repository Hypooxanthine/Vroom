#pragma once

#if defined(VRM_PLATFORM_LINUX)
  #define VRM_SCRIPT_API
#elif defined(VRM_PLATFORM_WINDOWS)
  #define VRM_SCRIPT_API
#endif

#if defined(VRM_PLATFORM_LINUX)
  #define VRM_SCRIPT_API_C __attribute__ ((visibility ("default")))
#elif defined(VRM_PLATFORM_WINDOWS)
  #define VRM_SCRIPT_API_C __declspec(dllexport)
#else
  #define VRM_SCRIPT_API_C
#endif
