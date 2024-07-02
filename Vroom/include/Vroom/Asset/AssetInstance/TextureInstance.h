#pragma once

#include "Vroom/Asset/AssetInstance/AssetInstance.h"

namespace vrm
{

class TextureAsset;

class TextureInstance : public AssetInstance
{
public:
    TextureInstance();
    TextureInstance(TextureAsset* textureAsset);
    ~TextureInstance() = default;

    TextureAsset* getStaticAsset();
    const TextureAsset* getStaticAsset() const;
};

} // namespace vrm

