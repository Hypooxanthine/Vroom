#include "Vroom/Tools/RuntimeLibrary.h"

#include "Vroom/Core/Assert.h"

using namespace vrm;

RuntimeLibrary::RuntimeLibrary()
{
  _init();
}

RuntimeLibrary::~RuntimeLibrary()
{
  _cleanup();
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
