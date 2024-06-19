#include "Vroom/Asset/AssetInstance/AssetInstance.h"

#include "Vroom/Asset/StaticAsset/StaticAsset.h"

namespace vrm
{

AssetInstance::AssetInstance(StaticAsset* staticAsset)
    : m_StaticAsset(staticAsset)
{
    m_StaticAsset->notifyNewInstance();
}

AssetInstance::AssetInstance(const AssetInstance& other)
    : m_StaticAsset(other.m_StaticAsset)
{
    m_StaticAsset->notifyNewInstance();
}

AssetInstance& AssetInstance::operator=(const AssetInstance& other)
{
    if (this == &other)
        return *this;

    if (m_StaticAsset != nullptr)
        m_StaticAsset->notifyDeleteInstance();
    m_StaticAsset = other.m_StaticAsset;
    m_StaticAsset->notifyNewInstance();

    return *this;
}

AssetInstance::AssetInstance(AssetInstance&& other)
    : m_StaticAsset(other.m_StaticAsset)
{
    other.m_StaticAsset = nullptr;
}

AssetInstance& AssetInstance::operator=(AssetInstance&& other)
{
    if (this == &other)
        return *this;

    if (m_StaticAsset != nullptr)
        m_StaticAsset->notifyDeleteInstance();
    m_StaticAsset = other.m_StaticAsset;
    other.m_StaticAsset = nullptr;

    return *this;
}

AssetInstance::~AssetInstance()
{
    m_StaticAsset->notifyDeleteInstance();
    m_StaticAsset = nullptr;
}

} // namespace vrm