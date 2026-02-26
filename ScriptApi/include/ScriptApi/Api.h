#pragma once

#if defined(VRM_PLATFORM_LINUX)
  #define VRM_EXPORT_SYMBOLS __attribute__ ((visibility ("default")))
#elif defined(VRM_PLATFORM_WINDOWS)
  #define VRM_EXPORT_SYMBOLS __declspec(dllexport)
#else
  static_assert(false, "Unsupported platform");
#endif
