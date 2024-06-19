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

    template <typename T>
    T::InstanceType getAsset(const std::string& assetID)
    {
        if (!isAssetLoaded(assetID))
        {
            auto asset = std::make_unique<T>();
            VRM_ASSERT_MSG(asset->load(assetID), "Failed to load asset: {}", assetID);

            m_Assets[assetID] = std::move(asset);
        }

        return dynamic_cast<T*>(m_Assets[assetID].get())->createInstance();
    }

    bool isAssetLoaded(const std::string& assetID)
    {
        return m_Assets.contains(assetID);
    }

private:
    std::unordered_map<std::string, std::unique_ptr<StaticAsset>> m_Assets;

};

} // namespace vrm
