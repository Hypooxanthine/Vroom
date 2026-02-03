#pragma once

#if defined(VRM_PLATFORM_LINUX)
  #if defined(VRM_STATIC)
    #define VRM_SCRIPT_API
  #else
    #define VRM_SCRIPT_API __attribute__ ((visibility ("default")))
  #endif
#elif defined(VRM_PLATFORM_WINDOWS)
  #if defined(VRM_STATIC)
    #define VRM_SCRIPT_API
  #elif defined(VRM_TARGET_SCRIPT_LIBRARY)
    #define VRM_SCRIPT_API __declspec(dllexport)
  #else
    #define VRM_SCRIPT_API __declspec(dllimport)
  #endif
#else
  #define VRM_SCRIPT_API
#endif
