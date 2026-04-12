#pragma once

#include "AssetManager/Json.h"

#include "AssetManager/SceneNodeData.h"

namespace nlohmann
{

  NLOHMANN_JSON_SERIALIZE_ENUM(
    vrm::SceneNodeData::EType,
    {
      { vrm::SceneNodeData::EType::eRoot  , "Root"   },
      { vrm::SceneNodeData::EType::eEntity, "Entity" },
    }
  )

  void VRM_ASSETMANAGER_API to_json(json& j, const vrm::SceneNodeData& node);

  void VRM_ASSETMANAGER_API from_json(const json& j, vrm::SceneNodeData& node);

}
