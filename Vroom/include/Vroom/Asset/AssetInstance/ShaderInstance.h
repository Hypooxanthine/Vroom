#pragma once

#include "Vroom/Asset/AssetInstance/AssetInstance.h"

namespace vrm
{

class ShaderAsset;

class ShaderInstance : public AssetInstance
{
public:
    ShaderInstance();
    ShaderInstance(ShaderAsset* meshAsset);
    ~ShaderInstance() = default;

    ShaderAsset* getStaticAsset();
    const ShaderAsset* getStaticAsset() const;
};

} // namespace vrm

