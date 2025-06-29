#pragma once

#include <list>

#include "Vroom/Api.h"
#include "Vroom/Asset/StaticAsset/StaticAsset.h"
#include "Vroom/Asset/StaticAsset/MaterialAsset.h"
#include "Vroom/Asset/AssetHandle.h"
#include "Vroom/Asset/AssetData/MeshData.h"
#include "Vroom/Render/RenderObject/RenderMesh.h"

struct aiNode;
struct aiMesh;
struct aiScene;

namespace vrm
{

  class VRM_API MeshAsset : public StaticAsset
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
  
    void _processNode(aiNode* node, const aiScene* scene);
    void _processMesh(aiMesh* mesh, const aiScene* scene);

  private:
    std::list<SubMesh> m_SubMeshes;
  };

} // namespace vrm