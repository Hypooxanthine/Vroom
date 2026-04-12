#include "Scene/SkyboxComponent.h"

#include "AssetManager/AssetManager.h"
#include "AssetManager/ComponentData.h"

using namespace vrm;

SkyboxComponent::SkyboxComponent()
{
  auto cubemap =
    AssetManager::Get().getAsset<CubemapAsset>("Resources/Engine/Cubemaps/DefaultSkybox/DefaultSkybox.json");
  setCubemapAsset(cubemap);
}

SkyboxComponent::SkyboxComponent(const SkyboxComponentData& data)
{
  setCubemapAsset(AssetManager::Get().getAsset<CubemapAsset>(data.resourceName));
}

SkyboxComponent::SkyboxComponent(CubemapAsset::Handle cubemap)
{
  setCubemapAsset(cubemap);
}

SkyboxComponent::~SkyboxComponent()
{}

SkyboxComponentData SkyboxComponent::getData() const
{
  SkyboxComponentData data;
  data.resourceName = getCubemapAsset()->getFilePath();

  return data;
}
