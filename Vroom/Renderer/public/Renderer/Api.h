#pragma once

#if defined(__GNUC__)
  #if defined(VRM_RENDERER_EXPORTS)
    #define VRM_RENDERER_API __attribute__ ((visibility ("default")))
  #else
    #define VRM_RENDERER_API
  #endif
#elif defined(_MSC_VER) 
  #if defined(VRM_RENDERER_EXPORTS)
    #define VRM_RENDERER_API __declspec(dllexport)
  #else
    #define VRM_RENDERER_API
  #endif
#else
  #define VRM_RENDERER_API
#endif
