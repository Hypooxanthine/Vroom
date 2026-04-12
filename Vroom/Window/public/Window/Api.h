#pragma once

#if defined(__GNUC__)
  #if defined(VRM_WINDOW_EXPORTS)
    #define VRM_WINDOW_API __attribute__ ((visibility ("default")))
  #else
    #define VRM_WINDOW_API
  #endif
#elif defined(_MSC_VER) 
  #if defined(VRM_WINDOW_EXPORTS)
    #define VRM_WINDOW_API __declspec(dllexport)
  #else
    #define VRM_WINDOW_API
  #endif
#else
  #define VRM_WINDOW_API
#endif
