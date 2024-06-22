#include "Vroom/Asset/AssetInstance/MaterialInstance.h"

#include "Vroom/Asset/StaticAsset/MaterialAsset.h"

namespace vrm
{

MaterialInstance::MaterialInstance()
    : AssetInstance()
{
}

MaterialInstance::MaterialInstance(MaterialAsset* materialAsset)
    : AssetInstance(materialAsset)
{
}

MaterialInstance::~MaterialInstance()
{
}

MaterialAsset* MaterialInstance::getStaticAsset()
{
    return static_cast<MaterialAsset*>(m_StaticAsset);
}

const MaterialAsset* MaterialInstance::getStaticAsset() const
{
    return static_cast<const MaterialAsset*>(m_StaticAsset);
}

} // namespace vrm