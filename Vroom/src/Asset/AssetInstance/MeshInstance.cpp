#include "Vroom/Asset/AssetInstance/MeshInstance.h"

#include "Vroom/Asset/StaticAsset/MeshAsset.h"

namespace vrm
{

MeshInstance::MeshInstance()
    : AssetInstance()
{
}

MeshInstance::MeshInstance(MeshAsset* meshAsset)
    : AssetInstance(meshAsset)
{
}

MeshAsset* MeshInstance::getStaticAsset()
{
    return static_cast<MeshAsset*>(m_StaticAsset);
}

const MeshAsset* MeshInstance::getStaticAsset() const
{
    return static_cast<MeshAsset*>(m_StaticAsset);
}

} // namespace vrm