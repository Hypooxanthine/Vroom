#pragma once

#include <unordered_map>
#include <set>
#include <string>

#include <glm/glm.hpp>

#include "Vroom/Scene/Components/PointLightComponent.h"
#include "Vroom/Render/RawShaderData/SSBOPointLightData.h"
#include "Vroom/Render/Abstraction/StorageBuffer.h"

namespace vrm
{

  class LightRegistry
  {
  public:
    LightRegistry();
    LightRegistry(const LightRegistry &) = delete;
    LightRegistry(LightRegistry &&) = default;
    ~LightRegistry() {}

    LightRegistry &operator=(const LightRegistry &) = delete;
    LightRegistry &operator=(LightRegistry &&) = default;

    void submitPointLight(const PointLightComponent &pointLight, const glm::vec3 &position, size_t identifier);
    void updatePointLight(const PointLightComponent &pointLight, const glm::vec3 &position, size_t identifier);
    void removePointLight(size_t identifier);

    void update();
    inline bool isDirty() const { return m_dirty; }

    inline const gl::AutoResizeStorageBuffer& getPointLightsStorageBuffer() const { return m_ssbo; }

  private:
    size_t getValidAddress();
    inline void markDirty() { m_dirty = true; }

    void updateGpuSize();

    SSBOPointLightData& getPointLight(size_t identifier);

  private:
    static constexpr GLintptr s_ssboOffset = sizeof(uint32_t);
    static constexpr GLsizei s_pointLightSize = sizeof(SSBOPointLightData);

    bool m_dirty = false;

    std::vector<SSBOPointLightData> m_lights;
    std::unordered_map<size_t, size_t> m_idToAddress;
    std::set<size_t> m_freeAddresses;
    std::vector<bool> m_activeLights;

    gl::AutoResizeStorageBuffer m_ssbo;
  };

} // namespace vrm
