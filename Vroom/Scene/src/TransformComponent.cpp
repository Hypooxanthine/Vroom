#include "Scene/TransformComponent.h"

#include "AssetManager/ComponentData.h"

using namespace vrm;

TransformComponent::TransformComponent(const TransformComponentData& data)
{
  setPosition(data.position);
  setScale(data.scale);
  setRotation(data.rotation);
}

TransformComponentData TransformComponent::getData() const
{
  TransformComponentData data;
  data.position = getPosition();
  data.scale    = getScale();
  data.rotation = getRotation();

  return data;
}
