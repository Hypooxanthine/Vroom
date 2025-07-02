#pragma once

#include "Vroom/Asset/Parsing/Json.h"

#include "Vroom/Asset/AssetData/SceneNodeData.h"

namespace nlohmann
{

  NLOHMANN_JSON_SERIALIZE_ENUM(
    vrm::SceneNodeData::EType,
    {
      { vrm::SceneNodeData::EType::eRoot  , "Root"   },
      { vrm::SceneNodeData::EType::eEntity, "Entity" },
    }
  )

  void VRM_API to_json(json& j, const vrm::SceneNodeData& node);

  void VRM_API from_json(const json& j, vrm::SceneNodeData& node);

}
