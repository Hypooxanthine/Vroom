#pragma once

#include <glm/glm.hpp>

#include "AssetManager/ComponentDataFwds.h"
#include "Scene/Api.h"

namespace vrm
{

struct VRM_SCENE_API DirectionalLightComponent
{
  DirectionalLightComponent() = default;

  explicit DirectionalLightComponent(const DirectionalLightComponentData& data);

  DirectionalLightComponentData getData() const;

  glm::vec3 color        = glm::vec3(1.0f);
  float     intensity    = 0.5f;
  bool      castsShadows = true;
};

} // namespace vrm
