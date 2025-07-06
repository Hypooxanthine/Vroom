#pragma once

#include <unordered_map>
#include <set>
#include <string>
#include <array>

#include <glm/glm.hpp>

#include "Vroom/DataStructure/LinearRegistry.h"
#include "Vroom/Render/AutoBuffer.h"

namespace vrm
{
  struct PointLightComponent;
  struct DirectionalLightComponent;

  class LightRegistry
  {
  public:

    struct RawDirLight
    {
      glm::mat4 viewProj;
      glm::vec4 direction;
      glm::vec4 color;
      float intensity;
      uint32_t castsShadows;
      float _pad[2];
    };

    struct RawPointLight
    {
      glm::vec4 position;
      glm::vec4 color;
      float intensity;
      float radius;
      float smoothRadius;
      float constantAttenuation;
      float linearAttenuation;
      float quadraticAttenuation;
      float _pad[2];
    };

    using DirLightRegistry = LinearRegistry<RawDirLight, size_t>;
    using PointLightRegistry = LinearRegistry<RawPointLight, size_t>;

  public:
    LightRegistry();
    LightRegistry(const LightRegistry&) = delete;
    LightRegistry(LightRegistry&&) = default;
    ~LightRegistry() {}

    LightRegistry& operator=(const LightRegistry&) = delete;
    LightRegistry& operator=(LightRegistry&&) = default;

    inline const DirLightRegistry& getDirectionalLights() const { return m_dirLightsRegistry; }
    inline const PointLightRegistry& getPointLights() const { return m_pointLightsRegistry; }

    void startRegistering();
    void endRegistering();

    void submitLight(const PointLightComponent& pointLight, const glm::vec3& position, size_t identifier);

    void submitLight(const DirectionalLightComponent& dirLight, const glm::vec3& direction, size_t identifier);

    inline const gl::Buffer& getPointLightsStorageBuffer() const { return m_pointLightsBuffer.getBuffer(); }
    
    inline const gl::Buffer& getDirLightsStorageBuffer() const { return m_dirLightsBuffer.getBuffer(); }

  private:

    void _updateGpuDirLights();

    void _updateGpuPointLights();

  private:

    DirLightRegistry m_dirLightsRegistry;
    PointLightRegistry m_pointLightsRegistry;

    render::AutoBuffer m_dirLightsBuffer, m_pointLightsBuffer;
  };

} // namespace vrm
