#pragma once

#include "Vroom/Asset/StaticAsset/StaticAsset.h"
#include "Vroom/Asset/AssetInstance/ComputeShaderInstance.h"

namespace vrm
{

class ComputeShaderAsset : public StaticAsset
{
public:
    using InstanceType = ComputeShaderInstance;

public:
    ComputeShaderAsset();
    ~ComputeShaderAsset();

    [[nodiscard]] ComputeShaderInstance createInstance();

protected: 
    bool loadImpl(const std::string& filePath, AssetManager& manager) override;

private:
};

} // namespace vrm