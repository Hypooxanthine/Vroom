#pragma once

#include "glm/ext/vector_float3.hpp"
#include <glm/glm.hpp>

#include "AssetManager/ComponentDataFwds.h"
#include "Scene/Api.h"

namespace vrm
{

class VRM_SCENE_API PointLightComponent
{
public:

  PointLightComponent();

  explicit PointLightComponent(const PointLightComponentData& data);

  PointLightComponentData getData() const;

  const glm::vec3& getColor() const
  {
    return color;
  }

  void setColor(const glm::vec3& newValue)
  {
    color = newValue;
    m_dirtyForRender = true;
  }

  float getIntensity() const
  {
    return intensity;
  }

  void setIntensity(float newValue)
  {
    intensity = newValue;
    m_dirtyForRender = true;
  }

  float getRadius() const
  {
    return radius;
  }

  void setRadius(float newValue)
  {
    radius = newValue;
    m_dirtyForRender = true;
  }

  float getSmoothRadius() const
  {
    return smoothRadius;
  }

  void setSmoothRadius(float newValue)
  {
    smoothRadius = newValue;
    m_dirtyForRender = true;
  }

  float getConstantAttenuation() const
  {
    return constantAttenuation;
  }

  void setConstantAttenuation(float newValue)
  {
    constantAttenuation = newValue;
    m_dirtyForRender = true;
  }

  float getLinearAttenuation() const
  {
    return linearAttenuation;
  }

  void setLinearAttenuation(float newValue)
  {
    linearAttenuation = newValue;
    m_dirtyForRender = true;
  }

  float getQuadraticAttenuation() const
  {
    return quadraticAttenuation;
  }

  void setQuadraticAttenuation(float newValue)
  {
    quadraticAttenuation = newValue;
    m_dirtyForRender = true;
  }

  bool consumeDirtyForRender() const
  {
    bool value = m_dirtyForRender;
    m_dirtyForRender = false;

    return value;
  }

  void markDirtyForRender() const
  {
    m_dirtyForRender = true;
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
      bool value = plc.consumeDirtyForRender();

      return value;
    }
  };

private:

  mutable bool m_dirtyForRender = true;

  glm::vec3 color = glm::vec3(1.0f);
  float intensity = 5.0f;
  float radius = 30.0f;
  float smoothRadius = 0.8f;
  float constantAttenuation = 1.0f;
  float linearAttenuation = 0.0f;
  float quadraticAttenuation = 0.1f;
};

} // namespace vrm
