#pragma once

#include <vector>

#include "AssetManager/Api.h"
#include "AssetManager/SceneNodeData.h"

namespace vrm
{

  class Entity;

  struct VRM_ASSETMANAGER_API SceneData
  {
    
    std::vector<SceneNodeData> nodes;

  };

} // namespace vrm
