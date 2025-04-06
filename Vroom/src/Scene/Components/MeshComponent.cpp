#include "Vroom/Scene/Components/MeshComponent.h"

#include "Vroom/Asset/Asset.h"

using namespace vrm;

MeshComponent::MeshComponent()
  : MeshComponent(AssetManager::Get().getAsset<MeshAsset>("Resources/Engine/Meshes/default_cube.obj"))
{

}

MeshComponent::MeshComponent(const MeshAsset::Handle& meshInstance)
{
  setMesh(meshInstance);
}

const MeshAsset::Handle& MeshComponent::getMesh() const
{
  return m_MeshInstance;
}

void MeshComponent::setMesh(const MeshAsset::Handle& meshInstance)
{
  m_MeshInstance = meshInstance;
  
  // By default, mesh component materials are the ones defines in the mesh asset
  m_Materials.clear(meshInstance->getSubMeshes().size());

  uint8_t slot = 0;
  for (const auto& submesh : meshInstance->getSubMeshes())
  {
    m_Materials.setMaterial(slot++, submesh.materialInstance);
  }
}

void MeshComponent::setMaterial(size_t slot, MaterialAsset::Handle mat)
{
  m_Materials.setMaterial(slot, mat);
}
