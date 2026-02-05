#ifdef _WIN32

#include <windows.h>

#include "Vroom/Core/Log.h"
#include "Vroom/Tools/RuntimeLibrary.h"

#define IMPL (*m_impl)

using namespace vrm;

struct RuntimeLibrary::Impl
{
  inline Impl() {}
  inline ~Impl() {}

  HMODULE handle = nullptr;
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

  // Convert path to wide string (UTF-16) because LoadLibraryW expects LPCWSTR
  std::wstring widePath = path.wstring();

  IMPL.handle = LoadLibraryW(widePath.c_str());
  if (!IMPL.handle)
  {
    DWORD errorCode = GetLastError();
    VRM_LOG_ERROR("Could not open runtime library {}. Error code: {}", path.string(), errorCode);
    return false;
  }

  return true;
}

void RuntimeLibrary::_unload()
{
  if (!isLoaded())
    return;

  if (!FreeLibrary(IMPL.handle))
  {
    DWORD errorCode = GetLastError();
    VRM_LOG_WARN("Error while unloading runtime library. Error code: {}", errorCode);
  }

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

  FARPROC symbol = GetProcAddress(IMPL.handle, name.c_str());
  if (!symbol)
  {
    DWORD errorCode = GetLastError();
    VRM_LOG_ERROR("Could not load symbol {}. Error code: {}", name, errorCode);
    return nullptr;
  }

  return reinterpret_cast<void*>(symbol);
}

#endif
