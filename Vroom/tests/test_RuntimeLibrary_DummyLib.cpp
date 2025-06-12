

#define EXPORT extern "C"

EXPORT int Identity(int i)
{
  return i;
}

EXPORT int TimesTwo(int i)
{
  return i * 2;
}

EXPORT int GetSeven()
{
  return 7;
}
