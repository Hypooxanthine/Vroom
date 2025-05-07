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

  for (const auto& node : data.getNodes())
  {
    if (node.type == SceneData::SceneNode::EType::eRoot)
    {
      VRM_CHECK_RET_FALSE_MSG(!foundRoot, "You cannot specify 2 root nodes");

      // A root entity is already provided by the Scene
      renameRoot(node.name);
      VRM_CHECK_RET_FALSE_MSG(node.parent.empty(), "Root node cannot specify a parent node");
      VRM_CHECK_RET_FALSE_MSG(node.components.empty(), "Root not cannot hold components");
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
      VRM_CHECK_RET_FALSE_MSG(compData->addToEntity(entity), "Could not add component to entity");
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

void Scene::addNodeComponents(const Entity& e, SceneData& data, size_t nodeID) const
{
  if (const auto* c = m_Registry.try_get<TransformComponent>(e); c != nullptr)
  {
    auto cd = std::make_unique<SceneData::TransformComponent>();

    cd->position = c->getPosition();
    cd->rotation = c->getRotation();
    cd->scale = c->getScale();

    data.addComponent(nodeID, std::move(cd));
  }

  if (const auto* c = m_Registry.try_get<MeshComponent>(e); c != nullptr)
  {
    auto cd = std::make_unique<SceneData::MeshComponent>();

    cd->resourceName = c->getMesh()->getFilePath();
    cd->castsShadow = c->doesCastShadow();

    data.addComponent(nodeID, std::move(cd));
  }

  if (const auto* c = m_Registry.try_get<DirectionalLightComponent>(e); c != nullptr)
  {
    auto cd = std::make_unique<SceneData::DirectionalLightComponent>();

    cd->color = c->color;
    cd->intensity = c->intensity;
    cd->castsShadows = c->castsShadows;

    data.addComponent(nodeID, std::move(cd));
  }

  if (const auto* c = m_Registry.try_get<PointLightComponent>(e); c != nullptr)
  {
    auto cd = std::make_unique<SceneData::PointLightComponent>();

    cd->color = c->color;
    cd->intensity = c->intensity;
    cd->radius = c->radius;

    data.addComponent(nodeID, std::move(cd));
  }

  if (const auto* c = m_Registry.try_get<ScriptHandler>(e); c != nullptr)
  {
    auto cd = std::make_unique<SceneData::ScriptComponent>();

    cd->resourceName = c->getScript().getScriptName();

    data.addComponent(nodeID, std::move(cd));
  }
}

void Scene::addNodeRecursive(const Entity& e, SceneData& data, const std::string& parent, const SceneData::SceneNode::EType& nodetype) const
{
  const std::string& name = e.getName();
  size_t id = data.addNode(name, nodetype, parent);
  addNodeComponents(e, data, id);

  for (const auto& child : e.getChildren())
  {
    addNodeRecursive(child, data, name, SceneData::SceneNode::EType::eEntity);
  }
}

SceneData Scene::getSceneData() const
{
  SceneData data;

  addNodeRecursive(getRoot(), data, "", SceneData::SceneNode::EType::eRoot);

  return std::move(data);
}
