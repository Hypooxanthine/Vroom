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

  inline float getIntensity() const
  {
    return intensity;
  }

  inline void setColor(const glm::vec3& newValue)
  {
    color            = newValue;
    m_dirtyForRender = true;
  }

  inline void setIntensity(float newValue)
  {
    intensity        = newValue;
    m_dirtyForRender = true;
  }

  inline void setCastsShadows(bool newValue)
  {
    castsShadows     = newValue;
    m_dirtyForRender = true;
  }

  inline bool consumeDirtyForRender() const
  {
    bool value       = m_dirtyForRender;
    m_dirtyForRender = false;

    return value;
  }

  inline void markDirtyForRender() const
  {
    m_dirtyForRender = true;
  }

  glm::vec3 color        = glm::vec3(1.0f);
  float     intensity    = 0.5f;
  bool      castsShadows = true;

private:

  mutable bool m_dirtyForRender = true;
};

} // namespace vrm
