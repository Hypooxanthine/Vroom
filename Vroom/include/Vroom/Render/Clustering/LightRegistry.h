#pragma once

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <list>
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
    LightRegistry(const LightRegistry&) = delete;
    LightRegistry(LightRegistry&&) = default;
    virtual ~LightRegistry() {}

    LightRegistry& operator=(const LightRegistry&) = delete;
    LightRegistry& operator=(LightRegistry&&) = default;

    void beginFrame();

    void submitPointLight(const PointLightComponent& pointLight, const glm::vec3& position, const std::string& identifier);

    void endFrame();

    const std::unordered_map<int, SSBOPointLightData>& getPointLights() const { return m_PointLights; }

    inline const gl::AutoResizeStorageBuffer& getPointLightsStorageBuffer() const { return m_SSBOPointLights; }

private:
    void updateData();

private:
    // Point lights that are currently in the scene
    std::unordered_map<int, SSBOPointLightData> m_PointLights;
    // SSBO for point lights
    gl::AutoResizeStorageBuffer m_SSBOPointLights;
    // Identifiers point to the address of the light in the SSBO
    std::unordered_map<std::string, int> m_PointLightAddresses;

    std::unordered_map<std::string, SSBOPointLightData> m_AddedPointLights;
    std::unordered_set<int> m_FreePointLightAdresses;
    int m_NextPointLightAddress = sizeof(int);
};

} // namespace vrm
