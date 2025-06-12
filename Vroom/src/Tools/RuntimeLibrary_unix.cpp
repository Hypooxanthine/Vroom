#ifdef __unix__

#include <dlfcn.h>

#include "Vroom/Core/Assert.h"
#include "Vroom/Tools/RuntimeLibrary.h"

#define IMPL (*m_impl)

using namespace vrm;

struct RuntimeLibrary::Impl
{
  inline Impl() {}
  inline ~Impl() {}

  void* handle = nullptr;
};

void RuntimeLibrary::_init()
{
  _cleanup();
  m_impl = new Impl();
}

void RuntimeLibrary::_cleanup()
{
  if (m_impl)
  {
    delete m_impl;
    m_impl = nullptr;
  }
}

bool RuntimeLibrary::_load(const std::filesystem::path& path)
{
  unload();

  dlerror();
  IMPL.handle = dlopen(std::filesystem::canonical(path).c_str(), RTLD_LAZY);
  if (!IMPL.handle)
  {
    VRM_LOG_ERROR("Could not open runtime library {} . Error log: {}", path.string(), dlerror());
    return false;
  }

  return true;
}

void RuntimeLibrary::_unload()
{
  if(!isLoaded())
    return;

  dlerror();
  dlclose(IMPL.handle);

  const char* err = dlerror();
  if (err)
    VRM_LOG_WARN("Error while unloading runtime library. Error log: {}", err);

  IMPL.handle = nullptr;
}

bool RuntimeLibrary::_isLoaded() const
{
  return IMPL.handle != nullptr;
}

void* RuntimeLibrary::_getSymbol(const std::string& name)
{
  if (!isLoaded())
    return nullptr;
  
  dlerror();
  void* symbol = dlsym(IMPL.handle, name.c_str());
  const char* err = dlerror();

  if (err)
  {
    VRM_LOG_ERROR("Could not load symbol {} . Error log: {}", name, err);
    return nullptr;
  }

  return symbol;
}

#endif
