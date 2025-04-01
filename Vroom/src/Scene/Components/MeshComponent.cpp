#include "Vroom/Scene/Components/MeshComponent.h"

#include "Vroom/Asset/Asset.h"

using namespace vrm;

MeshComponent::MeshComponent()
  : MeshComponent(AssetManager::Get().getAsset<MeshAsset>("Resources/Engine/Meshes/default_cube.obj"))
{

}

MeshComponent::MeshComponent(const MeshAsset::Handle& meshInstance)
  : m_MeshInstance(meshInstance)
{
}

const MeshAsset::Handle& MeshComponent::getMesh() const
{
  return m_MeshInstance;
}

void MeshComponent::setMesh(const MeshAsset::Handle& meshInstance)
{
  m_MeshInstance = meshInstance;
}
