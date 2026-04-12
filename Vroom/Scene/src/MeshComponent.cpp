#include "Scene/MeshComponent.h"

#include "AssetManager/AssetManager.h"
#include "AssetManager/ComponentData.h"

using namespace vrm;

MeshComponent::MeshComponent()
  : MeshComponent(AssetManager::Get().getAsset<MeshAsset>("Resources/Engine/Meshes/default_cube.obj"))
{}

MeshComponent::MeshComponent(const MeshComponentData& data)
{
  if (!data.resourceName.empty())
    setMesh(AssetManager::Get().getAsset<MeshAsset>(data.resourceName));
  setCastsShadow(data.castsShadow);
  setVisible(data.visible);
}

MeshComponent::MeshComponent(const MeshAsset::Handle& meshInstance)
{
  setMesh(meshInstance);
}

MeshComponentData MeshComponent::getData() const
{
  MeshComponentData data;
  data.resourceName = getMesh()->getFilePath();
  data.castsShadow  = doesCastShadow();
  data.visible      = isVisible();

  return data;
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
