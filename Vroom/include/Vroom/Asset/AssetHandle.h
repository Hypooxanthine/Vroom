#pragma once

#include <concepts>
#include <cstdint>

namespace vrm
{

  template <typename T>
  class AssetHandle final
  {
  public:

    inline AssetHandle()
      : m_assetRef(nullptr)
    {
    }

    inline AssetHandle(const T& asset)
      : m_assetRef(&asset)
    {
      notifyNew();
    }

    inline ~AssetHandle()
    {
      notifyDelete();
    }

    inline AssetHandle(const AssetHandle& other)
      : AssetHandle(*other.m_assetRef)
    {

    }

    inline AssetHandle& operator=(const AssetHandle& other)
    {
      if (m_assetRef != other.m_assetRef)
      {
        notifyDelete();

        m_assetRef = other.m_assetRef;

        notifyNew();
      }

      return *this;
    }

    inline AssetHandle(AssetHandle&& other) noexcept
    {
      m_assetRef = other.m_assetRef;
      other.m_assetRef = nullptr;
    }

    inline AssetHandle& operator=(AssetHandle&& other) noexcept
    {
      if (m_assetRef != other.m_assetRef)
      {
        notifyDelete();

        m_assetRef = other.m_assetRef;
        other.m_assetRef = nullptr;
      }

      return *this;
    }

    inline bool isValid() const
    {
      return m_assetRef != nullptr;
    }

    inline const T& get() const
    {
      return *m_assetRef;
    }

    inline uintptr_t getPtr() const
    {
      return reinterpret_cast<uintptr_t>(m_assetRef);
    }

    inline const T* operator->() const
    {
      return m_assetRef;
    }

  private:

    inline void notifyNew()
    {
      if (m_assetRef) m_assetRef->notifyNewInstance();
    }

    inline void notifyDelete()
    {
      if (m_assetRef) m_assetRef->notifyDeleteInstance();
    }

  private:
    const T* m_assetRef;

  };

} // namespace vrm
