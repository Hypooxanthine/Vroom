#pragma once

#include "Vroom/Asset/StaticAsset/StaticAsset.h"
#include "Vroom/Asset/AssetInstance/ImageInstance.h"

namespace vrm
{

class ImageAsset : public StaticAsset
{
public:
    using InstanceType = ImageInstance;
public:
    ImageAsset() = default;
    ~ImageAsset() = default;

    [[nodiscard]] ImageInstance createInstance();

protected:
    bool loadImpl(const std::string& filePath) override;
};

} // namespace vrm