#include "Vroom/Scene/Components/MeshComponent.h"

#include "Vroom/Asset/Asset.h"

namespace vrm
{

MeshComponent::MeshComponent(const MeshInstance& meshInstance)
    : m_MeshInstance(meshInstance)
{
}

const MeshInstance& MeshComponent::getMesh() const
{
    return m_MeshInstance;
}

void MeshComponent::setMesh(const MeshInstance& meshInstance)
{
    m_MeshInstance = meshInstance;
}

} // namespace vrm
