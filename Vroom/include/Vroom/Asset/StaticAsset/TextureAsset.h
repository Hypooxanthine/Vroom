#pragma once

#include <Vroom/Asset/StaticAsset/StaticAsset.h>
#include <Vroom/Asset/AssetInstance/TextureInstance.h>

namespace vrm
{

class TextureAsset : public StaticAsset
{
public:
    using InstanceType = TextureInstance;

public:
    TextureAsset();
    ~TextureAsset();

    [[nodiscard]] TextureInstance createInstance();

protected:
    bool loadImpl(const std::string& filePath, AssetManager& manager) override;
};

} // namespace vrm