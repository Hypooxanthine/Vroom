#include "AssetManager/ComponentData.h"
#include "AssetManager/SceneAsset.h"
#include "Core/Assert.h"
#include "Scene/DirectionalLightComponent.h"
#include "Scene/Entity.h"
#include "Scene/MeshComponent.h"
#include "Scene/PointLightComponent.h"
#include "Scene/Scene.h"
#include "Scene/ScriptComponent.h"
#include "Scene/ScriptHandler.h"
#include "Scene/SkyboxComponent.h"
#include "Scene/TransformComponent.h"
#include "ScriptEngine/ScriptEngine.h"

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

    if (const auto* compData = node.tryGetComponent<TransformComponentData>(); compData != nullptr)
    {
      entity.getComponent<TransformComponent>() = TransformComponent(*compData);
    }

    if (const auto* compData = node.tryGetComponent<MeshComponentData>(); compData != nullptr)
    {
      entity.addComponent<MeshComponent>(*compData);
    }

    if (const auto* compData = node.tryGetComponent<SkyboxComponentData>(); compData != nullptr)
    {
      entity.addComponent<SkyboxComponent>(*compData);
    }

    if (const auto* compData = node.tryGetComponent<DirectionalLightComponentData>(); compData != nullptr)
    {
      entity.addComponent<DirectionalLightComponent>(*compData);
    }

    if (const auto* compData = node.tryGetComponent<PointLightComponentData>(); compData != nullptr)
    {
      entity.addComponent<PointLightComponent>(*compData);
    }

    if (const auto* compData = node.tryGetComponent<ScriptComponentData>(); compData != nullptr)
    {
      VRM_CHECK_MSG(ScriptEngine::Get().isScriptRegistered(compData->resourceName), "Script {} is not registered",
                    compData->resourceName);
      entity.addScriptComponent(compData->resourceName);
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
    data.emplaceComponent<TransformComponentData>(c->getData());
  }

  if (const auto* c = m_Registry.try_get<MeshComponent>(e); c != nullptr)
  {
    data.emplaceComponent<MeshComponentData>(c->getData());
  }

  if (const auto* c = m_Registry.try_get<SkyboxComponent>(e); c != nullptr)
  {
    data.emplaceComponent<SkyboxComponentData>(c->getData());
  }

  if (const auto* c = m_Registry.try_get<DirectionalLightComponent>(e); c != nullptr)
  {
    data.emplaceComponent<DirectionalLightComponentData>(c->getData());
  }

  if (const auto* c = m_Registry.try_get<PointLightComponent>(e); c != nullptr)
  {
    data.emplaceComponent<PointLightComponentData>(c->getData());
  }

  if (const auto* c = m_Registry.try_get<ScriptHandler>(e); c != nullptr)
  {
    data.emplaceComponent<ScriptComponentData>(c->getData());
  }
}

void Scene::addNodeRecursive(const Entity& e, SceneData& data, const std::string& parent,
                             const SceneNodeData::EType& nodetype) const
{
  SceneNodeData nodeData;
  nodeData.name   = e.getName();
  nodeData.type   = nodetype;
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

  return data;
}
