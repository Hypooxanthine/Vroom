#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "Vroom/Api.h"
#include "Vroom/Core/Assert.h"
#include "Vroom/Asset/StaticAsset/StaticAsset.h"
#include "Vroom/Asset/AssetHandle.h"

namespace vrm
{

  class VRM_API AssetManager
  {
  public:
    AssetManager(const AssetManager&) = delete;
    AssetManager(AssetManager&&) = delete;
    AssetManager& operator=(const AssetManager&) = delete;
    AssetManager& operator=(AssetManager&&) = delete;
    ~AssetManager();

    /**
     * @brief Initialize the asset manager.
     *
     */
    static void Init();

    /**
     * @brief Shutdown the asset manager.
     *
     */
    static void Shutdown();

    /**
     * @brief Get the instance of the asset manager.
     *
     * @return AssetManager& The instance of the asset manager.
     */
    static AssetManager& Get();

    /**
     * @brief Get an asset by its ID. If the asset is not loaded, it will be loaded first.
     * You may want to load multiple assets at the start of the application to avoid loading times during gameplay.
     *
     * @tparam T The type of the asset to get. Must be a subclass of StaticAsset.
     * @param assetID The ID of the asset to get.
     * @return T::InstanceType  An instance of the asset.
     */
    template <typename T>
    AssetHandle<T> getAsset(const std::string& assetID)
    {
      loadAsset<T>(assetID);

      T* asT = dynamic_cast<T*>(m_Assets.at(assetID).get());
      VRM_DEBUG_ASSERT(asT != nullptr);

      return AssetHandle<T>{ *asT };
    }

    /**
     * @brief  Load an asset of type T with the given ID.
     *
     * @tparam T  The type of the asset to load. Must be a subclass of StaticAsset.
     * @param assetID  The ID of the asset to load.
     */
    template <typename T>
    void loadAsset(const std::string& assetID)
    {
      VRM_ASSERT_MSG(tryLoadAsset<T>(assetID), "Failed to load asset: {}", assetID);
    }

    template <typename T>
    void reloadAsset(const std::string& assetID)
    {
      VRM_ASSERT_MSG(tryReloadAsset<T>(assetID), "Failed to load asset: {}", assetID);
    }

    template <typename T>
    bool tryLoadAsset(const std::string& assetID)
    {
      return isAssetLoaded(assetID) || tryReloadAsset<T>(assetID);
    }

    template <typename T>
    inline bool tryReloadAsset(const std::string& assetID)
    {
      auto asset = std::make_unique<T>();
      if (!asset->load(assetID))
      {
        return false;
      }

      m_Assets[assetID] = std::move(asset);

      return true;
    }

    /**
     * @brief Check if an asset is loaded.
     *
     * @param assetID The ID of the asset to check.
     * @return true If the asset is loaded.
     * @return false If the asset is not loaded.
     */
    bool isAssetLoaded(const std::string& assetID)
    {
      return m_Assets.contains(assetID);
    }

  private:
    AssetManager() = default;

  private:
    static std::unique_ptr<AssetManager> s_Instance;

    std::unordered_map<std::string, std::unique_ptr<StaticAsset>> m_Assets;

  };

} // namespace vrm
