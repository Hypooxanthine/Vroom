#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "Vroom/Core/Assert.h"
#include "Vroom/Asset/Asset.h"

namespace vrm
{

class AssetManager
{
public:
    AssetManager() = default;
    ~AssetManager() = default;

    /**
     * @brief Get an asset by its ID. If the asset is not loaded, it will be loaded first.
     * You may want to load multiple assets at the start of the application to avoid loading times during gameplay.
     * 
     * @tparam T The type of the asset to get. Must be a subclass of StaticAsset.
     * @param assetID The ID of the asset to get.
     * @return T::InstanceType  An instance of the asset.
     */
    template <typename T>
    T::InstanceType getAsset(const std::string& assetID)
    {
        loadAsset<T>(assetID);

        T* asT = dynamic_cast<T*>(m_Assets[assetID].get());
        VRM_DEBUG_ASSERT(asT != nullptr);

        return asT->createInstance();
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
        if (!isAssetLoaded(assetID))
        {
            auto asset = std::make_unique<T>();
            VRM_ASSERT_MSG(asset->load(assetID), "Failed to load asset: {}", assetID);

            m_Assets[assetID] = std::move(asset);
        }
    }

    bool isAssetLoaded(const std::string& assetID)
    {
        return m_Assets.contains(assetID);
    }

private:
    std::unordered_map<std::string, std::unique_ptr<StaticAsset>> m_Assets;

};

} // namespace vrm
