#include "Vroom/Asset/StaticAsset/StaticAsset.h"

#include "Vroom/Asset/AssetInstance/AssetInstance.h"

namespace vrm
{

void StaticAsset::notifyNewInstance()
{
    m_InstanceCount++;
}

void StaticAsset::notifyDeleteInstance()
{
    m_InstanceCount--;
}

size_t StaticAsset::getInstanceCount() const
{
    return m_InstanceCount;
}

bool StaticAsset::load(const std::string& filePath)
{
    return loadImpl(filePath);
}

} // namespace vrm