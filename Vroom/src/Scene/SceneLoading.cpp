#include "Vroom/Scene/Scene.h"

#include <nlohmann/json.hpp>

#include "Vroom/Asset/StaticAsset/SceneAsset.h"
#include "Vroom/Core/Assert.h"

#define CHECK(x, ...) VRM_CHECK_RET_FALSE_MSG(x, __VA_ARGS__)

using namespace vrm;
using json = nlohmann::json;

static bool HandleSceneNode(Scene& scene, const json& sceneNode)
{

}

bool Scene::loadFromAsset(const SceneInstance& data)
{
  destroyAllEntities();
  const auto& jsonRoot = data.getStaticAsset()->getSceneData();
  CHECK(jsonRoot.contains("scene_nodes"), "Couldn't find json node scene_nodes");

  for (const auto& sceneNode : jsonRoot["scene_nodes"])
  {
    HandleSceneNode(*this, sceneNode);
  }
}