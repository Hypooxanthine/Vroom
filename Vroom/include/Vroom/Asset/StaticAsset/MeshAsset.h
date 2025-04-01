#pragma once

#include <list>

#include "Vroom/Asset/StaticAsset/StaticAsset.h"
#include "Vroom/Asset/StaticAsset/MaterialAsset.h"
#include "Vroom/Asset/AssetHandle.h"
#include "Vroom/Asset/AssetData/MeshData.h"
#include "Vroom/Render/RenderObject/RenderMesh.h"

namespace vrm
{

  class MeshAsset : public StaticAsset
  {
  public:
    using Handle = AssetHandle<MeshAsset>;
  public:

    struct SubMesh
    {
      SubMesh(RenderMesh&& render, MeshData&& data, MaterialAsset::Handle instance);

      RenderMesh renderMesh;
      MeshData meshData;
      MaterialAsset::Handle materialInstance;
    };

  public:
    MeshAsset();
    ~MeshAsset();

    const std::list<SubMesh>& getSubMeshes() const { return m_SubMeshes; }

    void addSubmesh(const MeshData& mesh, MaterialAsset::Handle instance);
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