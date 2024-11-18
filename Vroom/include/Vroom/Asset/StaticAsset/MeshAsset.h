#pragma once

#include <list>

#include "Vroom/Asset/StaticAsset/StaticAsset.h"
#include "Vroom/Asset/AssetInstance/MeshInstance.h"
#include "Vroom/Asset/AssetData/MeshData.h"
#include "Vroom/Render/RenderObject/RenderMesh.h"
#include "Vroom/Asset/AssetInstance/MaterialInstance.h"

namespace vrm
{

class MeshAsset : public StaticAsset
{
public:
    using InstanceType = MeshInstance;

    struct SubMesh
    {
        SubMesh(RenderMesh&& render, MeshData&& data, MaterialInstance instance);

        RenderMesh renderMesh;
        MeshData meshData;
        MaterialInstance materialInstance;
    };

public:
    MeshAsset();
    ~MeshAsset();

    [[nodiscard]] MeshInstance createInstance();

    const std::list<SubMesh>& getSubMeshes() const { return m_SubMeshes; }

    void addSubmesh(const MeshData& mesh, MaterialInstance instance);
    void addSubmesh(const MeshData& mesh);

    void clear();

protected: 
    bool loadImpl(const std::string& filePath) override;

private:
    bool loadObj(const std::string& filePath);

private:
    std::list<SubMesh> m_SubMeshes;
};

} // namespace vrm