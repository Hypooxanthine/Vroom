#pragma once

#if defined(VRM_PLATFORM_LINUX)
  #if defined(VRM_APPLICATION_EXPORTS)
    #define VRM_APPLICATION_API __attribute__ ((visibility ("default")))
  #else
    #define VRM_APPLICATION_API
  #endif
#elif defined(VRM_PLATFORM_WINDOWS) 
  #if defined(VRM_APPLICATION_EXPORTS)
    #define VRM_APPLICATION_API __declspec(dllexport)
  #else
    #define VRM_APPLICATION_API
  #endif
#else
  #define VRM_APPLICATION_API
#endif
