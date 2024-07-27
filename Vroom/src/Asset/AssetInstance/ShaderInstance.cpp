#include "Vroom/Asset/AssetInstance/ShaderInstance.h"

#include "Vroom/Asset/StaticAsset/ShaderAsset.h"

namespace vrm
{

ShaderInstance::ShaderInstance()
    : AssetInstance()
{
}

ShaderInstance::ShaderInstance(ShaderAsset* meshAsset)
    : AssetInstance(meshAsset)
{
}

ShaderAsset* ShaderInstance::getStaticAsset()
{
    return static_cast<ShaderAsset*>(m_StaticAsset);
}

const ShaderAsset* ShaderInstance::getStaticAsset() const
{
    return static_cast<const ShaderAsset*>(m_StaticAsset);
}

} // namespace vrm