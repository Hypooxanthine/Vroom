#pragma once

#include "Vroom/Asset/AssetInstance/AssetInstance.h"

namespace vrm
{

class ComputeShaderAsset;

class ComputeShaderInstance : public AssetInstance
{
public:
    ComputeShaderInstance();
    ComputeShaderInstance(ComputeShaderAsset* meshAsset);
    ~ComputeShaderInstance() = default;

    ComputeShaderAsset* getStaticAsset();
    const ComputeShaderAsset* getStaticAsset() const;
};

} // namespace vrm

