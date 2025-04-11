#pragma once

#include <unordered_map>
#include <set>
#include <string>
#include <array>

#include <glm/glm.hpp>

#include "Vroom/Render/RawShaderData/SSBOPointLightData.h"
#include "Vroom/Render/RawShaderData/SSBODirectionalLightData.h"
#include "Vroom/Render/Helpers/StorageBufferRegistry.h"
#include "Vroom/Render/Abstraction/StorageBuffer.h"

namespace vrm
{
  struct PointLightComponent;
  struct DirectionalLightComponent;

  class LightRegistry
  {
  public:
    LightRegistry();
    LightRegistry(const LightRegistry &) = delete;
    LightRegistry(LightRegistry &&) = default;
    ~LightRegistry() {}

    LightRegistry &operator=(const LightRegistry &) = delete;
    LightRegistry &operator=(LightRegistry &&) = default;

    void submitLight(const PointLightComponent &pointLight, const glm::vec3 &position, size_t identifier);
    void updateLight(const PointLightComponent &pointLight, const glm::vec3 &position, size_t identifier);
    void removePointLight(size_t identifier);

    void submitLight(const DirectionalLightComponent &dirLight, const glm::vec3 &direction, size_t identifier);
    void updateLight(const DirectionalLightComponent &dirLight, const glm::vec3 &direction, size_t identifier);
    void removeDirectionalLight(size_t identifier);

    void update();

    inline const gl::AutoResizeStorageBuffer& getPointLightsStorageBuffer() const { return m_ssbo; }

  private:

  private:
    static constexpr size_t s_maxDirLights = 10;
    static constexpr GLsizei s_dirLightSize = sizeof(SSBODirectionalLightData);
    static constexpr GLintptr s_dirLightOffset = 0;
    static constexpr GLintptr s_pointLightOffset = sizeof(uint32_t) + s_maxDirLights * s_dirLightSize;
    static constexpr GLsizei s_pointLightSize = sizeof(SSBOPointLightData);
    
    StorageBufferRegistry<SSBODirectionalLightData, size_t, s_maxDirLights> m_dirLightsRegistry;
    StorageBufferRegistry<SSBOPointLightData, size_t> m_pointLightsRegistry;

    gl::AutoResizeStorageBuffer m_ssbo;
  };

} // namespace vrm
