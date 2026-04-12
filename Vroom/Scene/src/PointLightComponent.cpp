#include "Scene/PointLightComponent.h"

#include "AssetManager/ComponentData.h"

using namespace vrm;

PointLightComponent::PointLightComponent()
{}

PointLightComponent::PointLightComponent(const PointLightComponentData& data)
{
  setColor(data.color);
  setIntensity(data.intensity);
  setRadius(data.radius);
  setSmoothRadius(data.smoothRadius);
  setConstantAttenuation(data.constantAttenuation);
  setLinearAttenuation(data.linearAttenuation);
  setQuadraticAttenuation(data.quadraticAttenuation);
}

PointLightComponentData PointLightComponent::getData() const
{
  PointLightComponentData data;
  data.color                = getColor();
  data.intensity            = getIntensity();
  data.radius               = getRadius();
  data.smoothRadius         = getSmoothRadius();
  data.constantAttenuation  = getConstantAttenuation();
  data.linearAttenuation    = getLinearAttenuation();
  data.quadraticAttenuation = getQuadraticAttenuation();

  return data;
}
