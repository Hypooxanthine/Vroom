#include "Vroom/Scene/Scene.h"

#include <nlohmann/json.hpp>

#include "Vroom/Asset/AssetManager.h"
#include "Vroom/Asset/StaticAsset/SceneAsset.h"
#include "Vroom/Asset/StaticAsset/MeshAsset.h"
#include "Vroom/Core/Assert.h"

#include "Vroom/Scene/Scene.h"
#include "Vroom/Scene/Entity.h"
#include "Vroom/Scene/Components/TransformComponent.h"
#include "Vroom/Scene/Components/MeshComponent.h"
#include "Vroom/Scene/Components/ScriptComponent.h"
#include "Vroom/Scene/Components/PointLightComponent.h"
#include "Vroom/Scene/Components/DirectionalLightComponent.h"

using namespace vrm;

bool Scene::loadFromAsset2(const SceneData& data)
{
  bool foundRoot = false;

  for (const auto& node : data.nodes)
  {
    if (node.type == SceneNodeData::EType::eRoot)
    {
      VRM_CHECK_RET_FALSE_MSG(!foundRoot, "You cannot specify 2 root nodes");

      // A root entity is already provided by the Scene
      renameRoot(node.name);
      VRM_CHECK_RET_FALSE_MSG(node.parent.empty(), "Root node cannot specify a parent node");
      VRM_CHECK_RET_FALSE_MSG(node.components.empty(), "Root cannot hold components");
      foundRoot = true;
      continue;
    }

    // If not root, we have to create the entity
    VRM_CHECK_RET_FALSE_MSG(!entityExists(node.name), "Entity with name {} already exists", node.name);
    Entity entity = createEntity(node.name);
    Entity father;

    if (!node.parent.empty())
    {
      if (!entityExists(node.parent))
      {
        VRM_LOG_ERROR("Could not set {}'s parent \"{}\": unknown entity", node.name, node.parent);
        return false;
      }

      father = getEntity(node.parent);
    }
    else
    {
      // No father specified => default is root
      father = getRoot();
    }

    setEntitiesRelation(father, entity);

    for (const auto& [_, compData] : node.components)
    {
      compData->addToEntity(entity);
    }
  }

  return true;
}

bool Scene::loadFromAsset(const SceneAsset::Handle& sceneAsset)
{
  destroyAllEntities();
  const auto& data = sceneAsset->getSceneData();

  if (!loadFromAsset2(data))
  {
    destroyAllEntities();
    VRM_LOG_ERROR("Could not load scene from asset data");
    return false;
  }

  VRM_LOG_INFO("Scene loaded successfully");

  return true;
}

void Scene::addNodeComponents(const Entity& e, SceneNodeData& data) const
{
  if (const auto* c = m_Registry.try_get<TransformComponent>(e); c != nullptr)
  {
    auto& cd = data.emplaceComponent<TransformComponentData>();
    cd.position = c->getPosition();
    cd.rotation = c->getRotation();
    cd.scale = c->getScale();
  }

  if (const auto* c = m_Registry.try_get<MeshComponent>(e); c != nullptr)
  {
    auto& mc = data.emplaceComponent<MeshComponentData>();
    mc.resourceName = c->getMesh()->getFilePath();
    mc.castsShadow = c->doesCastShadow();
  }

  if (const auto* c = m_Registry.try_get<DirectionalLightComponent>(e); c != nullptr)
  {
    auto& dlc = data.emplaceComponent<DirectionalLightComponentData>();
    dlc.color = c->color;
    dlc.intensity = c->intensity;
    dlc.castsShadows = c->castsShadows;
  }

  if (const auto* c = m_Registry.try_get<PointLightComponent>(e); c != nullptr)
  {
    auto& plc = data.emplaceComponent<PointLightComponentData>();
    plc.color = c->color;
    plc.intensity = c->intensity;
    plc.radius = c->radius;
    plc.constantAttenuation = c->constantAttenuation;
    plc.linearAttenuation = c->linearAttenuation;
    plc.quadraticAttenuation = c->quadraticAttenuation;
  }

  if (const auto* c = m_Registry.try_get<ScriptHandler>(e); c != nullptr)
  {
    auto& sc = data.emplaceComponent<ScriptComponentData>();
    sc.resourceName = c->getScript().getScriptName();
  }
}

void Scene::addNodeRecursive(const Entity& e, SceneData& data, const std::string& parent, const SceneNodeData::EType& nodetype) const
{
  SceneNodeData nodeData;
  nodeData.name = e.getName();
  nodeData.type = nodetype;
  nodeData.parent = parent;

  addNodeComponents(e, nodeData);

  // Add the node to the scene data
  data.nodes.emplace_back(std::move(nodeData));

  // Add children recursively
  for (const auto& child : e.getChildren())
  {
    addNodeRecursive(child, data, e.getName(), SceneNodeData::EType::eEntity);
  }
}

SceneData Scene::getSceneData() const
{
  SceneData data;

  addNodeRecursive(getRoot(), data, "", SceneNodeData::EType::eRoot);

  return std::move(data);
}
