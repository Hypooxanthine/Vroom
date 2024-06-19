#pragma once

#include "Vroom/Asset/StaticAsset/StaticAsset.h"
#include "Vroom/Asset/AssetInstance/MeshInstance.h"

namespace vrm
{

class MeshAsset : public StaticAsset
{
public:
    using InstanceType = MeshInstance;
public:
    MeshAsset() = default;
    ~MeshAsset() = default;

    [[nodiscard]] MeshInstance createInstance();

protected: 
    bool loadImpl(const std::string& filePath) override;
};

} // namespace vrm