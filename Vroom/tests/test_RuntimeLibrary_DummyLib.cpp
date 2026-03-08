


#if defined(__GNUC__)
  #define DUM_API __attribute__ ((visibility ("default")))
#elif defined(_MSC_VER)
  #define DUM_API __declspec(dllexport)
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
