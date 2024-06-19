#include "Vroom/Asset/AssetInstance/MeshInstance.h"

#include "Vroom/Asset/StaticAsset/MeshAsset.h"

namespace vrm
{

MeshInstance::MeshInstance(MeshAsset* meshAsset)
    : AssetInstance(meshAsset)
{
}

MeshAsset* MeshInstance::getStaticAsset()
{
    return static_cast<MeshAsset*>(m_StaticAsset);
}

} // namespace vrm