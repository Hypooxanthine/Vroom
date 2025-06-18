#pragma once

#include <filesystem>
#include <unordered_map>

#include "Vroom/Api.h"

namespace vrm
{

  class VRM_API RuntimeLibrary
  {
  public:
    RuntimeLibrary();
    ~RuntimeLibrary();

    inline static constexpr bool IsPlatformSupported()
    {
#if defined(__unix__)
      return true;
#endif
      return false;
    }

    bool load(const std::filesystem::path& path);
    void unload();

    template <typename Fn>
    inline Fn getSymbolCstyle(const std::string& name)
    {
      void* symbol = _tryGetCachedSymbol(name);

      if (!symbol)
        symbol = _getSymbol(name);

      return (Fn)symbol;
    }

    template <typename Signature>
    inline Signature* getSymbol(const std::string& name)
    {
      return getSymbolCstyle<Signature*>(name);
    }


    inline bool isLoaded() const { return _isLoaded(); }

  private:

    void* _tryGetCachedSymbol(const std::string& name);

  private: // Platform specific
    void _init();
    void _cleanup();

    bool _isLoaded() const;
    bool _load(const std::filesystem::path& path);
    void _unload();

    void* _getSymbol(const std::string& name);

  private:

    struct Impl;
    Impl* m_impl = nullptr;
    std::unordered_map<std::string, void*> m_cachedSymbols;
  };

}