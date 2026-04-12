#pragma once

#include <glm/glm.hpp>

#include "Rasterizer/AutoBuffer.h"
#include "RenderObjects/fwds.h"
#include "Tools/LinearRegistry.h"

namespace vrm
{

class LightRegistry
{
public:

  struct RawDirLight
  {
    glm::mat4 viewProj;
    glm::vec4 direction;
    glm::vec4 color;
    float     intensity;
    uint32_t  castsShadows;
    float     _pad[2];
  };

  struct RawPointLight
  {
    glm::vec4 position;
    glm::vec4 color;
    float     intensity;
    float     radius;
    float     smoothRadius;
    float     constantAttenuation;
    float     linearAttenuation;
    float     quadraticAttenuation;
    float     _pad[2];
  };

  using DirLightRegistry   = LinearRegistry<RawDirLight, size_t>;
  using PointLightRegistry = LinearRegistry<RawPointLight, size_t>;

public:

  LightRegistry();
  LightRegistry(const LightRegistry&) = delete;
  LightRegistry(LightRegistry&&)      = default;
  ~LightRegistry()
  {}

  LightRegistry& operator=(const LightRegistry&) = delete;
  LightRegistry& operator=(LightRegistry&&)      = default;

  inline const DirLightRegistry& getDirectionalLights() const
  {
    return m_dirLightsRegistry;
  }
  inline const PointLightRegistry& getPointLights() const
  {
    return m_pointLightsRegistry;
  }

  void startRegistering();
  void endRegistering();

  void submitLight(const render::PointLight& pointLight, size_t identifier);

  void submitLight(const render::DirectionalLight& dirLight, size_t identifier);

  inline const gl::Buffer& getPointLightsStorageBuffer() const
  {
    return m_pointLightsBuffer.getBuffer();
  }

  inline const gl::Buffer& getDirLightsStorageBuffer() const
  {
    return m_dirLightsBuffer.getBuffer();
  }

private:

  void _updateGpuDirLights();

  void _updateGpuPointLights();

private:

  DirLightRegistry   m_dirLightsRegistry;
  PointLightRegistry m_pointLightsRegistry;

  render::AutoBuffer m_dirLightsBuffer, m_pointLightsBuffer;
};

} // namespace vrm
