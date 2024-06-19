#include "Vroom/Asset/StaticAsset/MeshAsset.h"

#include "Vroom/Asset/AssetInstance/MeshInstance.h"

namespace vrm
{

MeshInstance MeshAsset::createInstance()
{
    return MeshInstance(this);
}

bool MeshAsset::loadImpl(const std::string& filePath)
{
    // Load mesh from file
    return true;
}

} // namespace vrm