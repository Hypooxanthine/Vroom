#include "Vroom/Scene/Components/SkyboxComponent.h"

#include "Vroom/Asset/AssetManager.h"
#include "Vroom/Asset/AssetData/CubemapData.h"

using namespace vrm;

SkyboxComponent::SkyboxComponent()
{
  auto cubemap = AssetManager::Get().getAsset<CubemapAsset>("Resources/Engine/Cubemaps/DefaultSkybox/DefaultSkybox.json");
  setCubemapAsset(cubemap);
}

SkyboxComponent::SkyboxComponent(CubemapAsset::Handle cubemap)
{
  setCubemapAsset(cubemap);
}

SkyboxComponent::~SkyboxComponent()
{

}
