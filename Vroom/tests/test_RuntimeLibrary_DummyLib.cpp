


#if defined(VRM_PLATFORM_LINUX)
  #if defined(DUM_STATIC)
    #define DUM_API
  #else
    #define DUM_API __attribute__ ((visibility ("default")))
  #endif
#elif defined(VRM_PLATFORM_WINDOWS)
  #if defined(DUM_STATIC)
    #define DUM_API
  #elif defined(DUM_TARGET_DUMMYLIB)
    #define DUM_API __declspec(dllexport)
  #else
    #define DUM_API __declspec(dllimport)
  #endif
#else
  #define DUM_API
#endif

extern "C"
{
  int DUM_API Identity(int i)
  {
    return i;
  }

  int DUM_API TimesTwo(int i)
  {
    return i * 2;
  }

  int DUM_API GetSeven()
  {
    return 7;
  }
}
