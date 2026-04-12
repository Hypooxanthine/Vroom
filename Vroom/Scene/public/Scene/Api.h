#pragma once

#if defined(__GNUC__)
  #if defined(VRM_SCENE_EXPORTS)
    #define VRM_SCENE_API __attribute__ ((visibility ("default")))
  #else
    #define VRM_SCENE_API
  #endif
#elif defined(_MSC_VER) 
  #if defined(VRM_SCENE_EXPORTS)
    #define VRM_SCENE_API __declspec(dllexport)
  #else
    #define VRM_SCENE_API
  #endif
#else
  #define VRM_SCENE_API
#endif
