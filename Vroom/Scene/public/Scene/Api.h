#pragma once

#if defined(VRM_PLATFORM_LINUX)
  #if defined(VRM_SCENE_EXPORTS)
    #define VRM_SCENE_API __attribute__ ((visibility ("default")))
  #else
    #define VRM_SCENE_API
  #endif
#elif defined(VRM_PLATFORM_WINDOWS) 
  #if defined(VRM_SCENE_EXPORTS)
    #define VRM_SCENE_API __declspec(dllexport)
  #else
    #define VRM_SCENE_API
  #endif
#else
  #define VRM_SCENE_API
#endif
