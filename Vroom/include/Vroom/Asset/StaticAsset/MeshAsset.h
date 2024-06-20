#pragma once

#include "Vroom/Asset/StaticAsset/StaticAsset.h"
#include "Vroom/Asset/AssetInstance/MeshInstance.h"
#include "Vroom/Asset/AssetData/MeshData.h"

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

    const MeshData& getMeshData() const;

protected: 
    bool loadImpl(const std::string& filePath) override;

private:
    bool loadObj(const std::string& filePath);

private:
    MeshData m_MeshData;
};

} // namespace vrm