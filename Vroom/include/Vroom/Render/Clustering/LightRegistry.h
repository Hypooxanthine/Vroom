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
    static constexpr size_t s_SBRAlignment = std::max(SSBODirectionalLightData::Alignment, SSBOPointLightData::Alignment);
    static constexpr size_t s_maxDirLights = 10;
    static constexpr GLsizei s_dirLightSize = sizeof(SSBODirectionalLightData);
    static constexpr GLintptr s_dirLightOffset = 0;
    static constexpr GLintptr s_pointLightOffset = s_SBRAlignment  + s_maxDirLights * s_dirLightSize;
    static constexpr GLsizei s_pointLightSize = sizeof(SSBOPointLightData);

    using DirLightsSBR = StorageBufferRegistry<SSBODirectionalLightData, size_t, s_maxDirLights>;
    using DirLightRegistry = DirLightsSBR::RegistryType;
    using PointLightsSBR = StorageBufferRegistry<SSBOPointLightData, size_t>;
    using PointLightRegistry = PointLightsSBR::RegistryType;

  public:
    LightRegistry();
    LightRegistry(const LightRegistry&) = delete;
    LightRegistry(LightRegistry&&) = default;
    ~LightRegistry() {}

    LightRegistry& operator=(const LightRegistry&) = delete;
    LightRegistry& operator=(LightRegistry&&) = default;

    inline const DirLightRegistry& getDirectionalLights() const { return m_dirLightsRegistry.getCPURegistry(); }
    inline const PointLightRegistry& getPointLights() const { return m_pointLightsRegistry.getCPURegistry(); }

    inline static consteval size_t GetMaxDirectionalLights() { return s_maxDirLights; }

    void startRegistering();
    void endRegistering();

    void submitLight(const PointLightComponent& pointLight, const glm::vec3& position, size_t identifier);

    void submitLight(const DirectionalLightComponent& dirLight, const glm::vec3& direction, size_t identifier);

    inline const gl::AutoResizeStorageBuffer& getPointLightsStorageBuffer() const { return m_ssbo; }

  private:

  private:

    DirLightsSBR m_dirLightsRegistry;
    PointLightsSBR m_pointLightsRegistry;

    gl::AutoResizeStorageBuffer m_ssbo;
  };

} // namespace vrm
