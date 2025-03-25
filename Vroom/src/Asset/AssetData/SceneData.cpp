#include "Vroom/Asset/AssetData/SceneData.h"

#include "Vroom/Asset/AssetManager.h"
#include "Vroom/Asset/StaticAsset/MeshAsset.h"

#include "Vroom/Scene/Entity.h"
#include "Vroom/Scene/Components/TransformComponent.h"
#include "Vroom/Scene/Components/MeshComponent.h"
#include "Vroom/Scene/Components/PointLightComponent.h"
#include "Vroom/Scene/Components/ScriptComponent.h"

using namespace vrm;

bool SceneData::TransformComponent::addToEntity(Entity& entity)
{
  auto& tc = entity.getComponent<::TransformComponent>();
  tc.setPosition(position);
  tc.setRotation(rotation);
  tc.setScale(scale);

  return true;
}

bool SceneData::MeshComponent::addToEntity(Entity& entity)
{
  auto& mc = entity.addComponent<::MeshComponent>();
  mc.setMesh(AssetManager::Get().getAsset<MeshAsset>(resourceName));

  return true;
}

bool SceneData::PointLightComponent::addToEntity(Entity& entity)
{
  auto& plc = entity.addComponent<::PointLightComponent>();
  plc.color = color;
  plc.intensity = intensity;
  plc.radius = radius;

  return true;
}

bool SceneData::ScriptComponent::addToEntity(Entity& entity)
{
  VRM_CHECK_RET_FALSE_MSG(ScriptEngine::Get().isScriptRegistered(resourceName), "Script {} is not registered", resourceName);
  std::unique_ptr<::ScriptComponent> sc;
  sc.reset(ScriptEngine::Get().createScriptComponent(resourceName));
  entity.addScriptComponent(std::move(sc));

  return true;
}
