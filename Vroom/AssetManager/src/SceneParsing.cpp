#include "AssetManager/SceneParsing.h"

using namespace vrm;

#include "AssetManager/Json.h"
#include "AssetManager/SceneNodeParsing.h"

namespace nlohmann
{

  void to_json(json& j, const SceneData& scene)
  {
    j = json::object();
    json& nodes = j["scene_nodes"] = json::array();

    for (const SceneNodeData& node : scene.nodes)
    {
      nodes.emplace_back() = node;
    }
  }

  void from_json(const json& j, SceneData& scene)
  {
    VRM_CHECK_THROW_MSG(j.is_object(), "Expected an object for SceneData, got {}", j.dump(2));
    VRM_CHECK_THROW_MSG(j.contains("scene_nodes"), "SceneData: Missing 'scene_nodes' field");

    const json& nodes = j["scene_nodes"];
    for (const auto& node : nodes)
    {
      VRM_CHECK_THROW_MSG(node.is_object(), "Unexpected scene node {}", node.dump(2));
      SceneNodeData& sceneNode = scene.nodes.emplace_back();
      sceneNode = node;
    }
  }

}
