#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <memory>
#include <unordered_map>

#include "Vroom/Api.h"
#include "Vroom/Asset/AssetData/SceneNodeData.h"
#include "Vroom/Asset/Parsing/Json.h"

namespace vrm
{

  class Entity;

  struct VRM_API SceneData
  {
    
    std::vector<SceneNodeData> nodes;

  };

} // namespace vrm
