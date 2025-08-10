#pragma once

#include <vector>

#include "Vroom/Api.h"
#include "Vroom/Asset/AssetData/SceneNodeData.h"

namespace vrm
{

  class Entity;

  struct VRM_API SceneData
  {
    
    std::vector<SceneNodeData> nodes;

  };

} // namespace vrm
