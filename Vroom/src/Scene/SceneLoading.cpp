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

bool Scene::loadFromAsset(const SceneInstance &sceneAsset)
{
  destroyAllEntities();
  const auto &data = sceneAsset.getStaticAsset()->getSceneData();

  if (!loadFromAsset2(data))
  {
    destroyAllEntities();
    VRM_LOG_ERROR("Could not load scene from asset data");
    return false;
  }

  VRM_LOG_INFO("Scene loaded successfully");

  return true;
}
