#pragma once

#include <glm/glm.hpp>

#include "Vroom/Api.h"
#include "glm/ext/vector_float3.hpp"

namespace vrm
{

class VRM_API PointLightComponent
{
public:

  PointLightComponent();

  inline const glm::vec3& getColor() const
  {
    return color;
  }

  inline void setColor(const glm::vec3& newValue)
  {
    color            = newValue;
    m_dirtyForRender = true;
  }

  inline float getIntensity() const
  {
    return intensity;
  }

  inline void setIntensity(float newValue)
  {
    intensity        = newValue;
    m_dirtyForRender = true;
  }

  inline float getRadius() const
  {
    return radius;
  }

  inline void setRadius(float newValue)
  {
    radius           = newValue;
    m_dirtyForRender = true;
  }

  inline float getSmoothRadius() const
  {
    return smoothRadius;
  }

  inline void setSmoothRadius(float newValue)
  {
    smoothRadius     = newValue;
    m_dirtyForRender = true;
  }

  inline float getConstantAttenuation() const
  {
    return constantAttenuation;
  }

  inline void setConstantAttenuation(float newValue)
  {
    constantAttenuation = newValue;
    m_dirtyForRender    = true;
  }

  inline float getLinearAttenuation() const
  {
    return linearAttenuation;
  }

  inline void setLinearAttenuation(float newValue)
  {
    linearAttenuation = newValue;
    m_dirtyForRender  = true;
  }

  inline float getQuadraticAttenuation() const
  {
    return quadraticAttenuation;
  }

  inline void setQuadraticAttenuation(float newValue)
  {
    quadraticAttenuation = newValue;
    m_dirtyForRender     = true;
  }

public:

  friend struct LightRegistryAttorney;

  struct LightRegistryAttorney
  {
  public:

    friend class LightRegistry;

  private:

    static bool getAndResetDirtyForRender(const PointLightComponent& plc)
    {
      bool value           = plc.m_dirtyForRender;
      plc.m_dirtyForRender = false;

      return value;
    }
  };

private:

  mutable bool m_dirtyForRender = true;

  glm::vec3 color                = glm::vec3(1.0f);
  float     intensity            = 5.0f;
  float     radius               = 30.0f;
  float     smoothRadius         = 0.8f;
  float     constantAttenuation  = 1.0f;
  float     linearAttenuation    = 0.0f;
  float     quadraticAttenuation = 0.1f;
};

} // namespace vrm
