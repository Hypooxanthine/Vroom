#include "Scene/DirectionalLightComponent.h"

#include "AssetManager/ComponentData.h"


using namespace vrm;

DirectionalLightComponent::DirectionalLightComponent(const DirectionalLightComponentData& data)
{
  setColor(data.color);
  setIntensity(data.intensity);
  setCastsShadows(data.castsShadows);
}

DirectionalLightComponentData DirectionalLightComponent::getData() const
{
  DirectionalLightComponentData data;
  data.color        = color;
  data.intensity    = intensity;
  data.castsShadows = castsShadows;

  return data;
}
