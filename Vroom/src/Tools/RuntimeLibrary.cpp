#include "Vroom/Tools/RuntimeLibrary.h"

using namespace vrm;

RuntimeLibrary::RuntimeLibrary()
{
  _init();
}

RuntimeLibrary::~RuntimeLibrary()
{
  _cleanup();
}

RuntimeLibrary::RuntimeLibrary(RuntimeLibrary&& other)
{
  *this = std::move(other);
}

RuntimeLibrary& RuntimeLibrary::operator=(RuntimeLibrary&& other)
{
  if (this != &other)
  {
    m_impl = other.m_impl;
    other.m_impl = nullptr;
    m_cachedSymbols = std::move(other.m_cachedSymbols);

    other._init();
  }

  return *this;
}

bool RuntimeLibrary::load(const std::filesystem::path& path)
{
  return _load(path);
}

void RuntimeLibrary::unload()
{
  m_cachedSymbols.clear();
  _unload();
}

void* RuntimeLibrary::_tryGetCachedSymbol(const std::string& name)
{
  auto it = m_cachedSymbols.find(name);

  return (it != m_cachedSymbols.end()) ? it->second : nullptr;
}
