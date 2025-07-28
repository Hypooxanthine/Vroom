#pragma once

#if defined(VRM_PLATFORM_LINUX)
  #if defined(VRM_STATIC)
    #define VRM_API
  #else
    #define VRM_API __attribute__ ((visibility ("default")))
  #endif
#elif defined(VRM_PLATFORM_WINDOWS)
  #if defined(VRM_STATIC)
    #define VRM_API
  #elif defined(VRM_TARGET_VROOM)
    #define VRM_API __declspec(dllexport)
  #else
    #define VRM_API __declspec(dllimport)
  #endif
#else
  #define VRM_API
#endif
