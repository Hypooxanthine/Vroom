#pragma once

#include <vector>

#include "AssetManager/Api.h"
#include "AssetManager/AssetHandle.h"
#include "AssetManager/MaterialAsset.h"
#include "AssetManager/StaticAsset.h"
#include "RenderObjects/Mesh.h"
#include "RenderObjects/MeshData.h"


struct aiNode;
struct aiMesh;
struct aiScene;

namespace vrm
{

class VRM_ASSETMANAGER_API MeshAsset : public StaticAsset
{
public:

  using Handle = AssetHandle<MeshAsset>;

public:

  struct SubMesh
  {
    SubMesh(render::Mesh&& render, MeshData&& data, MaterialAsset::Handle instance);

    render::Mesh          renderMesh;
    MeshData              meshData;
    MaterialAsset::Handle materialInstance;
  };

public:

  MeshAsset();
  ~MeshAsset();

  const std::vector<SubMesh>& getSubMeshes() const
  {
    return m_SubMeshes;
  }

  void addSubmesh(const MeshData& mesh, MaterialAsset::Handle instance);
  void addSubmesh(const MeshData& mesh);

  void clear();

protected:

  bool loadImpl(const std::string& filePath) override;

private:

  void _processNode(aiNode* node, const aiScene* scene);
  void _processMesh(aiMesh* mesh, const aiScene* scene);

private:

  std::vector<SubMesh> m_SubMeshes;
};

} // namespace vrm
