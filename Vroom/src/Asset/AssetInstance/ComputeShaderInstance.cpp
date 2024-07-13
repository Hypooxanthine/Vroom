#include "Vroom/Asset/AssetInstance/ComputeShaderInstance.h"

#include "Vroom/Asset/StaticAsset/ComputeShaderAsset.h"

namespace vrm
{

ComputeShaderInstance::ComputeShaderInstance()
    : AssetInstance()
{
}

ComputeShaderInstance::ComputeShaderInstance(ComputeShaderAsset* meshAsset)
    : AssetInstance(meshAsset)
{
}

ComputeShaderAsset* ComputeShaderInstance::getStaticAsset()
{
    return static_cast<ComputeShaderAsset*>(m_StaticAsset);
}

const ComputeShaderAsset* ComputeShaderInstance::getStaticAsset() const
{
    return static_cast<const ComputeShaderAsset*>(m_StaticAsset);
}

} // namespace vrm