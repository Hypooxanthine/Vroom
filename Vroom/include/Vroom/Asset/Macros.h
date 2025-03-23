#pragma once

namespace vrm
{

#define VRM_ASSET_INSTANCE_NAME(x) x##Instance
#define VRM_STATIC_ASSET_NAME(x) x##Asset

#define VRM_DECLARE_ASSET_INSTANCE_2(AssetInstanceName, StaticAssetName) \
  class StaticAssetName;\
  \
  class AssetInstanceName : public AssetInstance\
  {\
  public:\
    AssetInstanceName();\
    AssetInstanceName(StaticAssetName * staticAsset);\
    ~AssetInstanceName() = default;\
  \
    StaticAssetName *getStaticAsset();\
    const StaticAssetName *getStaticAsset() const;\
  }

#define VRM_DECLARE_ASSET_INSTANCE(assetRawName)\
  VRM_DECLARE_ASSET_INSTANCE_2(VRM_ASSET_INSTANCE_NAME(assetRawName), VRM_STATIC_ASSET_NAME(assetRawName))

#define VRM_IMPL_ASSET_INSTANCE_2(AssetInstanceName, StaticAssetName)\
  \
    AssetInstanceName::AssetInstanceName()\
        : AssetInstance()\
    {\
    }\
  \
    AssetInstanceName::AssetInstanceName(StaticAssetName* staticAsset)\
        : AssetInstance(staticAsset)\
    {\
    }\
  \
    StaticAssetName *AssetInstanceName::getStaticAsset()\
    {\
      return static_cast<StaticAssetName*>(m_StaticAsset);\
    }\
  \
    const StaticAssetName *AssetInstanceName::getStaticAsset() const\
    {\
      return static_cast<StaticAssetName*>(m_StaticAsset);\
    }\


#define VRM_IMPL_ASSET_INSTANCE(assetRawName)\
  VRM_IMPL_ASSET_INSTANCE_2(VRM_ASSET_INSTANCE_NAME(assetRawName), VRM_STATIC_ASSET_NAME(assetRawName))

#define VRM_DECLARE_STATIC_ASSET(assetRawName)\
  public:\
    using InstanceType = VRM_ASSET_INSTANCE_NAME(assetRawName);\
  \
    [[nodiscard]] VRM_ASSET_INSTANCE_NAME(assetRawName) createInstance();\
  protected:\
    bool loadImpl(const std::string& filePath) override;\
  private:\

#define VRM_IMPL_STATIC_ASSET(assetRawName)\
  vrm:: VRM_ASSET_INSTANCE_NAME(assetRawName) vrm:: VRM_STATIC_ASSET_NAME(assetRawName)::createInstance()\
  {\
    return VRM_ASSET_INSTANCE_NAME(assetRawName)(this);\
  }\

} // namespace vrm

