#pragma once

#include "Vroom/Asset/StaticAsset/StaticAsset.h"
#include "Vroom/Asset/AssetInstance/MeshInstance.h"
#include "Vroom/Asset/AssetData/MeshData.h"
#include "Vroom/Render/RenderObject/RenderMesh.h"

namespace vrm
{

class MeshAsset : public StaticAsset
{
public:
    using InstanceType = MeshInstance;
public:
    MeshAsset();
    ~MeshAsset();

    [[nodiscard]] MeshInstance createInstance();

    const MeshData& getMeshData() const;
    const RenderMesh& getRenderMesh() const;

protected: 
    bool loadImpl(const std::string& filePath) override;

private:
    bool loadObj(const std::string& filePath);

private:
    MeshData m_MeshData;
    std::unique_ptr<RenderMesh> m_RenderMesh;
};

} // namespace vrm